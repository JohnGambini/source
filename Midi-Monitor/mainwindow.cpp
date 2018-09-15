#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "midisettings.h"
#include "pm_common/portmidi.h"
#include "midiappcolumns.h"
#include "midiProcs.h"
#include "datamodel.h"

#include <QMessageBox>
#include <QFile>
#include <QTimer>
#include <QHeaderView>

QTimer inputEventTimer;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_applicationName = "MIDI Monitor";
    m_windowTitle = "MIDI Monitor[*]";
    m_applicationFilePath = "./MidiSequencerSettings.gna";
    m_projectFilesDirectory = "./";

    vector<gnaVariant> header;
    header.push_back("Time Stamp");
    header.push_back("Channel");
    header.push_back("MIDI Message");
    header.push_back("Value 1");
    header.push_back("Value 2");
    m_Root.setData(0,header[0]);
    m_Root.setData(1,header[1]);
    m_Root.setData(2,header[2]);
    m_Root.setData(3,header[3]);
    m_Root.setData(4,header[4]);

    DataModel * mdl = new DataModel(&m_Root);

    m_eventList = new EventListWidget(this);
    m_eventList->setModel(mdl);

    //QHeaderView * vhv = m_listView->verticalHeader();
    //vhv->setHidden(true);

    setCentralWidget(m_eventList);

    connect(ui->actionExit,SIGNAL(triggered()), this, SLOT(Exit()));
    connect(ui->actionClear,SIGNAL(triggered()),m_eventList,SLOT(clear()));
    connect(ui->actionSettings,SIGNAL(triggered()),this,SLOT(properties()));
    connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(About()));

    //This loads MIDI port info for this machine into the app from port midi
    loadApplicationSettings();

    //Start the midi timer
    Pt_Start(1,process_midi,0);

    //This loads MIDI port info into the port midi library
    Pm_Initialize();

    //Use application settings to get midi port information and open them
    openMidiPorts();

    app_to_midi = Pm_QueueCreate(16,sizeof(PortMsg));
    midi_to_app = Pm_QueueCreate(16,sizeof(PmEvent));

    connect(&inputEventTimer,SIGNAL(timeout()), this, SLOT(pollQueue()));
    inputEventTimer.start(15);

    midi_proc_active = true;



}

MainWindow::~MainWindow()
{
    saveApplicationSettings();
    midi_proc_active = false;
    Pt_Sleep(2);
    Pm_QueueDestroy(app_to_midi);
    closeMidiPorts();
    Pm_Terminate();
    Pt_Stop();
    delete ui;
}

void MainWindow::pollQueue()
{
    PmEvent buffer;
    PmError result;

    do
    {
        result = Pm_Dequeue(midi_to_app, &buffer);
        if (result == pmGotData)
        {
            m_eventList->addRow(buffer);
        }
    }
    while (result == pmGotData);

}

void MainWindow::Exit()
{
    if( isWindowModified())
    {
        QMessageBox msgBox;
        msgBox.setText("This document has been modified...");
        msgBox.setInformativeText("Do you want to save it before exiting the program.");
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);
        switch(msgBox.exec())
        {
        case QMessageBox::Save:
            //Save();
            qApp->quit();
            break;
        case QMessageBox::Discard:
            qApp->quit();
            break;
        case QMessageBox::Cancel:
            break;
        default:
            break;
        }

    }
    else
        qApp->quit();
}

//--------------------------------------------------------------------------------
//If this dialog closes with an accepted state
//Shut down all threads and start them again according
//to the new data.

void MainWindow::properties()
{
    MIDISettings * pDialog = new MIDISettings(this);
    //pDialog->m_errLog = &m_errLog;
    pDialog->setRoot(&m_applicationSettings);
    pDialog->exec();
    //if(pDialog->Accepted == true)
    //{
    //    m_sequencerView->setPortList(&m_applicationSettings);
    //}

}

void MainWindow::About()
{
    QString msg = "This is version 0.00001";
    QMessageBox::about(this, m_applicationName, msg );

}

//-------------------------------------------------------------------------------
//Either open an existing file and load previous application settings
//or build the application settings tree.

void MainWindow::loadApplicationSettings()
{
    QFile checkThis(m_applicationFilePath);
    if( checkThis.exists())
    {
        gnaFile aFile;
        if(aFile.Open(m_applicationFilePath.toStdString(), gna::FileRead))
        {
            m_applicationSettings.clear();
            m_applicationSettings.Serialize(aFile);
        }
        else
        {
            //m_filePath.clear();
            QMessageBox::critical(this,"Could not open file.",m_applicationFilePath);
        }
    }
    else
    {
        m_applicationSettings.clear();
        gnaVariant fps = 3;
        gnaVariant projDir = m_projectFilesDirectory.toStdString();
        m_applicationSettings.setData(0,fps);
        m_applicationSettings.setData(1,projDir);
        gnaVariant inName = "MIDI Input Ports";
        gnaVariant outName = "MIDI Output Ports";
        gnaTreeItem * inputPorts = m_applicationSettings.addChild(new gnaTreeItem(inName));
        gnaTreeItem * outputPorts = m_applicationSettings.addChild(new gnaTreeItem(outName));

        for( int i = 0; i < Pm_CountDevices(); i++)
        {
            const PmDeviceInfo * info = Pm_GetDeviceInfo(i);

            if(info->input == true)
            {
                gnaTreeItem * portInfo = inputPorts->addChild(new gnaTreeItem());
                portInfo->insertColumns(0,5);
                gnaVariant name;
                name = info->name;
                gnaVariant id = i;
                gnaVariant checked;
                checked = 0;
                //gnaVariant sendMMC = 1;
                //gnaVariant sendMTC = 1;
                portInfo->setData(0,name);
                portInfo->setData(PORTMIDI_ID, id);
                portInfo->setData(2,checked);
                //portInfo->setData(3,sendMMC);
                //portInfo->setData(4,sendMTC);
            }

            if(info->output == true)
            {
                gnaTreeItem * portInfo = outputPorts->addChild(new gnaTreeItem());
                portInfo->insertColumns(0,5);
                gnaVariant name;
                name = info->name;
                gnaVariant id = i;
                gnaVariant checked;
                checked = 0;
                gnaVariant sendMMC;
                sendMMC = 0;
                gnaVariant sendMTC;
                sendMTC = 0;
                portInfo->setData(0,name);
                portInfo->setData(PORTMIDI_ID, id);
                portInfo->setData(2,checked);
                portInfo->setData(3,sendMMC);
                portInfo->setData(4,sendMTC);
            }
        }
    }
}

void MainWindow::saveApplicationSettings()
{
    gnaFile aFile;
    if(aFile.Open(m_applicationFilePath.toStdString(), gna::FileWrite))
        m_applicationSettings.Serialize(aFile);
}

void MainWindow::openMidiPorts()
{
    gnaTreeItem * inputPorts = m_applicationSettings.child(MIDI_INPORTS);
    gnaTreeItem * outputPorts = m_applicationSettings.child(MIDI_OUTPORTS);
    PmError err;
    int i;
    char buf[128];
    memset(buf,1,128);

    for( i = 0; i < inputPorts->childCount() && i < 10; i++)
    {
        gnaTreeItem * port = inputPorts->child(i);
        gnaVariant portId = port->data(PORTMIDI_ID);
        gnaVariant isUsed = port->data(PORT_USED);
        gnaVariant midiOutIdx = i;
        port->setData(MIDI_IN_IDX,midiOutIdx);
        if(isUsed.toInt() == 0)
            continue;
        err = Pm_OpenInput( &midi_in_ports[i], portId.toInt(), NULL, 0, midiport_time_proc, 0 );
    }

    for( i = 0; i < outputPorts->childCount() && i < 10; i++)
    {
        gnaTreeItem * port = outputPorts->child(i);
        gnaVariant portId = port->data(PORTMIDI_ID);
        gnaVariant isUsed = port->data(PORT_USED);
        gnaVariant midiOutIdx = i;
        port->setData(MIDI_OUT_IDX,midiOutIdx);
        if(isUsed.toInt() == 0)
            continue;
        err = Pm_OpenOutput( &midi_out_ports[i], portId.toInt(), NULL, 0, midiport_time_proc, 0,0 );
        if(err == pmHostError)
        {
            Pm_GetHostErrorText(buf,128);
            ui->statusBar->setStatusTip(buf);
        }
    }
}

void MainWindow::closeMidiPorts()
{
    gnaTreeItem * inputPorts = m_applicationSettings.child(MIDI_INPORTS);
    gnaTreeItem * outputPorts = m_applicationSettings.child(MIDI_OUTPORTS);
    PmError err;
    int i;

    for( i = 0; i < inputPorts->childCount() && i < 10; i++)
    {
        gnaTreeItem * port = inputPorts->child(i);
        gnaVariant midiInIdx = port->data(MIDI_IN_IDX);
        err = Pm_Close( &midi_in_ports[midiInIdx.toInt()] );
    }

    for( i = 0; i < outputPorts->childCount() && i < 10; i++)
    {
        gnaTreeItem * port = outputPorts->child(i);
        gnaVariant midiOutIdx = port->data(MIDI_OUT_IDX);
        err = Pm_Close( &midi_out_ports[midiOutIdx.toInt()] );
    }
}
