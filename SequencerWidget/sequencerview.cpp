#include "sequencerview.h"

#include <QVBoxLayout>
#include <QSplitter>
#include <QThread>
#include <QScrollBar>
#include <QHeaderView>

#include "playclass.h"

#include "porttime/porttime.h"

SequencerView::SequencerView(QWidget *parent) :
    QWidget(parent)
{
    m_root = NULL;
    m_bStop = true;

    QVBoxLayout * layout = new QVBoxLayout(this);
    layout->setSpacing(0);
    layout->setMargin(0);

    QSplitter * splitter = new QSplitter;
    splitter->setHandleWidth(4);
    m_trackSettings = new TrackSettingsWidget;
    m_sequencerWidget = new SequencerWidget;
    splitter->addWidget(m_trackSettings);
    splitter->addWidget(m_sequencerWidget);

    layout->setMenuBar(&m_header);
    layout->addWidget(splitter);
    setLayout(layout);


    QList<int> iSizes2;
    iSizes2 << 298 << 800;
    splitter->setSizes(iSizes2);

    //House keeeping
    connect(m_trackSettings, SIGNAL(dataChanged()), this, SIGNAL(dataChanged()));
    connect(m_sequencerWidget, SIGNAL(dataChanged()), this, SIGNAL(dataChanged()));
    connect(&m_header,SIGNAL(buttonPressed(int)),this,SLOT(buttonPressed(int)));
    connect(&m_header,SIGNAL(pauseButton()),this,SLOT(pause()));
    connect(&m_header,SIGNAL(bpmChanged(int)),this,SLOT(bpmChange(int)));

    //Setup Scrollbars so that tracksettings and sequencerwidget scroll together
    QScrollBar * rightVScrollBar = m_sequencerWidget->verticalScrollBar();
    QScrollBar * leftVScrollBar = m_trackSettings->verticalScrollBar();
    connect(rightVScrollBar,SIGNAL(valueChanged(int)), leftVScrollBar, SLOT(setValue(int)));
    connect(leftVScrollBar,SIGNAL(valueChanged(int)), rightVScrollBar, SLOT(setValue(int)));

    //Resizing row height so that tracksettings and sequencerwidget resize together
    QHeaderView * vhv1 = m_trackSettings->verticalHeader();
    connect(vhv1,SIGNAL(sectionResized(int,int,int)),m_sequencerWidget,SLOT(resizeSection(int,int,int)));

    //Note labels button is in tracksettings, note widgets are in sequencerwidget. So...
    connect(m_trackSettings,SIGNAL(noteLabelsChanged(bool,int)),m_sequencerWidget,SLOT(setNoteLabels(bool,int)));

}

void SequencerView::addTrack(int row)
{
    int height = m_trackSettings->addTrack(row);
    m_sequencerWidget->addTrack(row,height);
}

void SequencerView::bpmChange(int val)
{
    if( m_root == NULL)
        return;

    gnaVariant data = val;
    m_root->setData(1, data );
    emit dataChanged();
}

void SequencerView::errorString(QString errMsg)
{
    m_header.setQueueLabel(errMsg);
}

void SequencerView::setRoot(gnaTreeItem * root)
{
    emit setThreadRoot(root);
    if(m_bStop == false)
        m_header.setQueueLabel("Sequence Queued...");
    gnaVariant bpmValue;
    gnaVariant stepsValue;

    m_root = root;

    if(m_root != NULL)
    {
        bpmValue = m_root->data(1);
        stepsValue = m_root->data(2);
    }

    if( bpmValue.toInt() == 0 )
    {
        bpmValue = 120;
        if(m_root)
        {
            m_root->setData(1,bpmValue);
        }
    }
    if(stepsValue.toInt() == 0)
    {
        stepsValue = 16;
        if(m_root)
        {
            m_root->setData(2,stepsValue);
        }
    }

    blockSignals(true);
    m_header.setSteps(stepsValue.toInt());
    m_header.setBPM(bpmValue.toInt());

    m_trackSettings->setRoot(root);
    m_sequencerWidget->setRoot(root);
    blockSignals(false);
}

void SequencerView::pause()
{
    m_bStop = true;
}

void SequencerView::buttonPressed(int id)
{
    QThread * thread;
    PlayClass * worker;

    //if(m_root == NULL)
    //    return;

    switch(id)
    {
    case 0:
        break;

    case 1: //playButton
        if( m_bStop )
        {
            thread = new QThread;
            worker = new PlayClass( m_root, &m_bStop, m_header.m_timeIndex );
            connect(thread, SIGNAL(started()), worker, SLOT(play()));
            //connect(worker, SIGNAL(finished()), thread, SLOT(quit()));
            //connect(worker, SIGNAL(finished()), worker, SLOT(deleteLater()));
            connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
            connect(worker,SIGNAL(clockTick(unsigned long)),&m_header,SLOT(setClock(unsigned long)));
            connect(worker,SIGNAL(processStopped()),&m_header, SLOT(timerThreadStopped()));
            connect(worker, SIGNAL(error(QString)), this, SLOT(errorString(QString)));
            worker->moveToThread(thread);
            m_bStop = false;
            thread->start(QThread::HighestPriority);

            /*
            thread = new QThread;
            worker = new PlayClass(m_root);
            int j = -1;
            for(int i = 0; (j = m_trackSettings->getPortID(i)) != -1; i++ )
                worker->addPort(j);
            worker->setBPM(m_header.BPM());
            connect(worker, SIGNAL(noteOn(int,int,int,int)), m_sequencerWidget, SLOT(noteOn(int,int,int,int)));
            connect(worker, SIGNAL(noteOff(int,int,int,int)), m_sequencerWidget, SLOT(noteOff(int,int,int,int)));
            connect(worker, SIGNAL(allNotesOff()),this,SLOT(allNotesOff()));
            //connect(worker, SIGNAL(stopped()), this, SLOT(stopPlaying()));
            connect(worker, SIGNAL(setQueueLabel(QString)), this, SLOT(setQueueLabel(QString)));
            connect(worker, SIGNAL(error(QString)), this, SLOT(errorString(QString)));
            connect(this, SIGNAL(stopButtonPressed()), worker, SLOT(stopPlaying()));
            connect(thread, SIGNAL(started()), worker, SLOT(play()));
            connect(worker, SIGNAL(finished()), thread, SLOT(quit()));
            connect(worker, SIGNAL(finished()), worker, SLOT(deleteLater()));
            connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
            connect(this, SIGNAL(setThreadRoot(gnaTreeItem*)),worker,SLOT(setRoot(gnaTreeItem*)));
            connect(worker,SIGNAL(tick(unsigned long)),&m_header,SLOT(setClock(unsigned long)));
            worker->moveToThread(thread);
            thread->start(QThread::HighestPriority);
            m_isPlaying = true;
            */
        }
        break;
    case 2:  //recordButton
        break;

    case 3: //stopButton
        m_bStop = true;
        m_header.setQueueLabel("");
        break;

    case 4:
        break;

    case 5:
        break;

    default:    // 10,12,14,16 steps buttons
        m_root->setData(2,id);
        emit dataChanged();
        break;
    }
}
