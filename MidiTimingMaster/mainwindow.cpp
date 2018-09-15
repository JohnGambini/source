#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "pm_common/portmidi.h"
#include "midisettings.h"
#include "midimasterstyle.h"
#include "midiProcs.h"
#include "midiappcolumns.h"

#include <QMessageBox>
#include <QVBoxLayout>
#include <QStyleFactory>
#include <QFile>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_errLog.Open("./errorLog.txt",gna::FileWrite);
    m_errLog.writeString("errorLog start.\r\n");

    m_projectFilesDirectory = "./";

    gnaVariant data = "MIDI Timming Master";
    m_applicationSettings.setData(0,data);

    setWindowTitle(data.toString());

    m_timingMaster = new TimingMasterWidget;

    setCentralWidget(m_timingMaster);

    connect(ui->actionMIDI_Settings,SIGNAL(triggered()), this, SLOT(ShowSettings()));
    connect(ui->actionExit,SIGNAL(triggered()), this, SLOT(Exit()));
    connect(ui->actionAbout,SIGNAL(triggered()), this, SLOT(About()));

    connect(m_timingMaster,SIGNAL(errMsg(QString,QString)), this, SLOT(errMsg(QString,QString)));
    connect(m_timingMaster,SIGNAL(buttonPressed(int)),this, SLOT(transportButtonPressed(int)));
    connect(m_timingMaster,SIGNAL(toErrLog(QString)),this,SLOT(errLog(QString)));

    Pm_Initialize();

    for( int i = 0; i < Pm_CountDevices(); i++)
    {
        const PmDeviceInfo * info = Pm_GetDeviceInfo(i);
        if(info->output == true)
        {
            gnaTreeItem * portInfo = m_applicationSettings.addChild(new gnaTreeItem());
            gnaVariant name = info->name;
            gnaVariant id = i;
            gnaVariant checked;
            checked = 0;
            gnaVariant sendMMC = 1;
            gnaVariant sendMTC = 1;
            portInfo->setData(0,name);
            portInfo->setData(1,id);
            portInfo->setData(2,checked);
            portInfo->setData(3,sendMMC);
            portInfo->setData(4,sendMTC);
        }
    }

    m_timingMaster->setRoot(&m_applicationSettings);

    //changeStyle("Motif");
    //changeStyle("CDE");
    //changeStyle("cleanLooks");
    //changeStyle("Plastique");
    //changePalette();

    QApplication::setStyle(new MidiMasterStyle);
    changePalette();

    //Start the midi timer
    Pt_Start( 2, timer_poll, 0 );

    //This loads MIDI port info into the port midi library
    Pm_Initialize();

    //This loads MIDI port info for this machine into the app from port midi
    loadApplicationSettings();


    //Use application settings to get midi port information and open them
    openMidiPorts();

    midi_proc_active = true;

    //Start threads for each midi port here
    //and have outputs wait for a MIDI_START or MIDI_CONTINUE
    //midi thru should start working right away.

}

MainWindow::~MainWindow()
{
    saveApplicationSettings();
    midi_proc_active = false;
    Pt_Sleep(2);
    closeMidiPorts();
    Pm_Terminate();
    Pt_Stop();
    delete ui;
}

void MainWindow::changeStyle(const QString &styleName)
{
    QApplication::setStyle(QStyleFactory::create(styleName));
}

void MainWindow::changePalette()
{
    QApplication::setPalette(QApplication::style()->standardPalette());
}

void MainWindow::ShowSettings()
{
    MIDISettings * pDialog = new MIDISettings(this);
    pDialog->setRoot(&m_applicationSettings);
    pDialog->setStyleBits(MIDISettings::OutputPorts);
    pDialog->exec();
    if(pDialog->Accepted == true)
    {
        m_timingMaster->setRoot(&m_applicationSettings);
    }
}

void MainWindow::Exit()
{
    qApp->quit();
}

void MainWindow::About()
{
    gnaVariant data = m_applicationSettings.data(0);
    QString msg = "<b>";
    msg += data.toString();
    msg += "<b>. This is version 0.00001";
    QString title = "About ";
    title += data.toString();
    QMessageBox::about(this, title, msg );
}

void MainWindow::errMsg(QString header, QString msg)
{
    QMessageBox::about(this, header, msg );
}

void MainWindow::errLog(QString errMsg)
{
    m_errLog.writeString(errMsg.toStdString().c_str());
}

void MainWindow::transportButtonPressed(int value)
{
    switch(value)
    {
    case 0: //Previous
        break;

    case 1: //Play
        send_start_stop = MIDI_CONTINUE;
        break;

    case 2: //Pause
        send_start_stop = MIDI_STOP;
        break;

    case 3: //Stop
        if(clock_is_running)
        {
            send_start_stop = MIDI_STOP;
            send_ssp = true;
        }
        break;

    case 4: //Record
        //send_midi_record = true;
        if(!clock_is_running)
            send_start_stop = MIDI_CONTINUE;
        break;

    case 5: //Next
        break;
    }
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
