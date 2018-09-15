#include "sequencerview.h"

#include <QVBoxLayout>
#include <QSplitter>
#include <QScrollBar>
#include <QHeaderView>

#include "playclass.h"

SequencerView::SequencerView(QWidget *parent) :
    QWidget(parent)
{
    m_isPlaying = false;
    m_root = NULL;
    QVBoxLayout * layout = new QVBoxLayout(this);
    layout->setSpacing(0);
    layout->setMargin(0);

    QSplitter * splitter = new QSplitter;
    splitter->setHandleWidth(4);
    m_trackSettings = new TrackSettingsWidget;
    m_sequencerWidget = new SequencerWidget;
    splitter->addWidget(m_trackSettings);
    splitter->addWidget(m_sequencerWidget);

    m_header = new SequencerHeader;

    layout->setMenuBar(m_header);
    layout->addWidget(splitter);
    setLayout(layout);

    QScrollBar * rightVScrollBar = m_sequencerWidget->verticalScrollBar();
    QScrollBar * leftVScrollBar = m_trackSettings->verticalScrollBar();

    connect(rightVScrollBar,SIGNAL(valueChanged(int)), leftVScrollBar, SLOT(setValue(int)));
    connect(leftVScrollBar,SIGNAL(valueChanged(int)), rightVScrollBar, SLOT(setValue(int)));
    connect(m_trackSettings, SIGNAL(dataChanged()), this, SIGNAL(dataChanged()));
    connect(m_sequencerWidget, SIGNAL(dataChanged()), this, SIGNAL(dataChanged()));
    connect(m_header,SIGNAL(bpmChanged(int)),this,SLOT(bpmChange(int)));
    connect(m_header,SIGNAL(buttonPressed(int)),this,SLOT(buttonPressed(int)));
    connect(m_trackSettings,SIGNAL(noteLabelsChanged(bool,int)),m_sequencerWidget,SLOT(setNoteLabels(bool,int)));
    connect(m_trackSettings,SIGNAL(rateChange(int,int)),m_sequencerWidget,SLOT(rateChange(int,int)));

    QHeaderView * vhv1 = m_trackSettings->verticalHeader();
    connect(vhv1,SIGNAL(sectionResized(int,int,int)),m_sequencerWidget,SLOT(resizeSection(int,int,int)));

    connect(m_trackSettings,SIGNAL(selectionChange(QModelIndexList&)),
            m_sequencerWidget,SLOT(setSelectedIndexes(QModelIndexList&)));
    connect(m_sequencerWidget,SIGNAL(selectionChange(QModelIndexList&)),
            m_trackSettings,SLOT(setSelectedIndexes(QModelIndexList&)));

    QList<int> iSizes2;
    iSizes2 << 140 << 800;
    splitter->setSizes(iSizes2);

    //setStyleSheet("QTableWidget { color:white;background-color:black }");

}

void SequencerView::addTrack(int row)
{
    int height = m_trackSettings->addTrack(row);
    m_sequencerWidget->addTrack(row,height);
}

void SequencerView::stopPlaying()
{
    m_header->setStopButton();
    m_isPlaying = false;
}

void SequencerView::bpmChange(int val)
{
    if( m_root == NULL)
        return;

    gnaVariant data = val;
    m_root->setData(1, data );
    emit dataChanged();
}

void SequencerView::stepsChange(int val)
{
    if( m_root == NULL)
        return;

    gnaVariant data = val;
    m_root->setData(2, data );
    emit dataChanged();

}

void SequencerView::channelChange(int,int)
{

}

void SequencerView::setRoot(gnaTreeItem * root)
{
    emit setThreadRoot(root);
    if(m_isPlaying)
        m_header->setQueueLabel("Sequence Queued...");
    gnaVariant bpmValue;
    gnaVariant stepsValue;

    m_root = root;

    if(m_root != NULL)
    {
        bpmValue = m_root->data(1);
        stepsValue = m_root->data(2);
    }
    if( bpmValue.toInt() == 0 )
        bpmValue = 120;
    if(stepsValue.toInt() == 0)
    {
        stepsValue = 16;
        if(m_root)
            m_root->setData(2,stepsValue);
    }

    blockSignals(true);
    m_header->setSteps(stepsValue.toInt());
    m_header->setBPM(bpmValue.toInt());

    m_trackSettings->setRoot(root);
    m_sequencerWidget->setRoot(root);
    blockSignals(false);
}

void SequencerView::buttonPressed(int id)
{
    QThread * thread = NULL;
    PlayClass * worker = NULL;

    if(m_root == NULL)
        return;

    switch(id)
    {
    case 0: //playButton
        if( !m_isPlaying )
        {
            thread = new QThread;
            worker = new PlayClass(m_root);
            int j = -1;
            for(int i = 0; (j = m_trackSettings->getPortID(i)) != -1; i++ )
                worker->addPort(j);
            worker->setBPM(m_header->BPM());
            connect(worker, SIGNAL(noteOn(int,int,int,int)), m_sequencerWidget, SLOT(noteOn(int,int,int,int)));
            connect(worker, SIGNAL(noteOff(int,int,int,int)), m_sequencerWidget, SLOT(noteOff(int,int,int,int)));
            connect(worker, SIGNAL(allNotesOff()),this,SLOT(allNotesOff()));
            connect(worker, SIGNAL(stopped()), this, SLOT(stopPlaying()));
            connect(worker, SIGNAL(setQueueLabel(QString)), this, SLOT(setQueueLabel(QString)));
            connect(worker, SIGNAL(error(QString)), this, SLOT(errorString(QString)));
            connect(this, SIGNAL(stopButtonPressed()), worker, SLOT(stopPlaying()));
            connect(thread, SIGNAL(started()), worker, SLOT(play()));
            connect(worker, SIGNAL(finished()), thread, SLOT(quit()));
            connect(worker, SIGNAL(finished()), worker, SLOT(deleteLater()));
            connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
            connect(this, SIGNAL(setThreadRoot(gnaTreeItem*)),worker,SLOT(setRoot(gnaTreeItem*)));
            worker->moveToThread(thread);
            thread->start(QThread::HighestPriority);
            m_isPlaying = true;
        }
        break;

    case 1: //stopButton
        emit stopButtonPressed();
        m_isPlaying = false;
        break;

    default:    // 10,12,14,16 steps buttons
        m_root->setData(2,id);
        break;
    }
}

void  SequencerView::setQueueLabel(QString val)
{
    m_header->setQueueLabel(val);
}

void SequencerView::itemDeleted(gnaTreeItem * delItem)
{
    if(m_root == delItem)
        setRoot(NULL);
}

void SequencerView::allNotesOff()
{
    for(int i = 0; i < m_sequencerWidget->rowCount(); i++)
    {
        for(int j = 0; j < 16; j++)
        {
            m_sequencerWidget->noteOff(j,i,0,0);
        }
    }
}

