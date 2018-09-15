#include "playclass.h"

#include <QTime>
#include <QCoreApplication>
#include <QEventLoop>

void delay1()
{
    QTime dieTime= QTime::currentTime().addMSecs(250);
    while( QTime::currentTime() < dieTime )
    QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

void delay2()
{
    QTime dieTime= QTime::currentTime().addMSecs(5);
    while( QTime::currentTime() < dieTime )
    QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

PlayClass::PlayClass(gnaTreeItem * root, QObject * parent) :
    QObject(parent)
{
    m_root = root;
    m_stop = false;
    m_noteOffset = 21;
    m_nextRoot = NULL;
    m_realPositionCount = 0;

    if(m_root)
        for(int i = 0; i < m_root->childCount(); i++)
        {
            m_trackPosition.push_back(0);
            m_rateCount.push_back(0);
            m_notesOff.push_back(new deque<structNotesOff *>);
            m_patches.push_back(-1);
        }
}

PlayClass::~PlayClass()
{
    for(vector<MIDIDriverWin32 *>::iterator drvList = m_midiDrivers.begin(); drvList < m_midiDrivers.end(); drvList++)
        delete (*drvList);

    for(vector<deque<structNotesOff *> * >::iterator childList = m_notesOff.begin(); childList < m_notesOff.end(); childList++)
    {
        for(deque<structNotesOff *>::iterator it = (*childList)->begin(); it < (*childList)->end(); it++ )
            delete (*it);

        delete (*childList);
    }

}

void PlayClass::addPort(int portId)
{
    m_portIds.push_back(portId);
    m_midiDrivers.push_back(new MIDIDriverWin32(64));
}


void PlayClass::play()
{
    QTimer timer;

    for(int i = 0; i < m_midiDrivers.size(); i++ )
        m_midiDrivers[i]->OpenMIDIOutPort(m_portIds[i]);

    delay1();

    connect(&timer, SIGNAL(timeout()), this, SLOT(timerEvent()));
    m_noteOnOff = true;
    timer.start(15000/m_BPM);
    while( m_stop == false)
    {
        delay1();
    }
    timer.stop();

    for( int i = 0; i < m_root->childCount(); i++ )
    {
        deque<structNotesOff *> * nof = m_notesOff[i];
        if( nof->size() > 0 )
        {
            structNotesOff * no = nof->front();
            MIDITimedBigMessage msg1;
            msg1.SetNoteOff((UCHAR)no->chnl, (UCHAR)no->note, (UCHAR)127);
            m_midiDrivers[no->port]->HardwareMsgOut(msg1);
            nof->pop_front();
        }
    }

    emit allNotesOff();

    delay1();

    for(int i = 0; i < m_midiDrivers.size(); i++ )
        m_midiDrivers[i]->CloseMIDIOutPort();
}

void PlayClass::setRoot( gnaTreeItem * newroot)
{
    m_nextRoot = newroot;
    if(newroot == NULL)
    {
        m_stop = true;
        emit setQueueLabel("");
        emit stopped();
    }
}

//emit signal with current (x,y track item), channel, midiport;
//for each track
void PlayClass::timerEvent()
{
    if(m_stop == true)
        return;

    gnaVariant steps = m_root->data(2);
    int iSteps = steps.toInt();
    int iRealPos = iSteps * 4;
    int iHalfPos = iSteps * 2;

    for( int i = 0; i < m_root->childCount(); i++ )
    {
        gnaTreeItem * trackItem = m_root->child(i);
        gnaVariant patch = trackItem->data(0);
        gnaVariant chnl = trackItem->data(1);
        gnaVariant port = trackItem->data(2);
        gnaVariant rate = trackItem->data(3);
        gnaVariant mute = trackItem->data(4);
        gnaVariant velcty = trackItem->data(5);

        gnaTreeItem * noteItem = trackItem->child(m_trackPosition[i]);
        gnaVariant note = noteItem->data(0);
        gnaVariant noteMute = noteItem->data(1);

        if( m_realPositionCount == 0)
            m_trackPosition[i] = 0;

        deque<structNotesOff *> * nof = m_notesOff[i];

        if( m_patches[i] != patch.toInt())
        {
            MIDITimedBigMessage msg1;
            msg1.SetProgramChange(chnl.toInt(), patch.toInt());
            m_midiDrivers[port.toInt()]->HardwareMsgOut(msg1);
            m_patches[i] = patch.toInt();
        }

        if( m_rateCount[i] == 0 && !noteMute.toInt())
        {
            if( nof->size() > 0 )
            {
                structNotesOff * no = nof->front();

                emit noteOff(no->pos,i,no->chnl, port.toInt());

                MIDITimedBigMessage msg1;
                msg1.SetNoteOff((UCHAR)no->chnl, (UCHAR)no->note, (UCHAR)127);
                m_midiDrivers[no->port]->HardwareMsgOut(msg1);

                nof->pop_front();
            }
        }

        if( !mute.toInt()&& m_rateCount[i] == 0 && !noteMute.toInt())
        {
            MIDITimedBigMessage msg1;
            msg1.SetNoteOn((UCHAR)chnl.toInt(), (UCHAR)note.toInt() + m_noteOffset, (UCHAR)127);
            msg1.SetVelocity((UCHAR)velcty.toInt()-1);
            m_midiDrivers[port.toInt()]->HardwareMsgOut(msg1);

            structNotesOff * no = new structNotesOff;
            no->pos = m_trackPosition[i];
            no->patch = patch.toInt();
            no->chnl = chnl.toInt();
            no->port = port.toInt();
            no->note = note.toInt() + m_noteOffset;
            nof->push_back(no);

            emit noteOn(m_trackPosition[i],i,chnl.toInt(),port.toInt());
        }

        if( m_rateCount[i] == 0)
        {
            m_trackPosition[i] = m_trackPosition[i] + 1;
            if( m_trackPosition[i] >= iSteps )
                m_trackPosition[i] = 0;
        }

        m_rateCount[i] = m_rateCount[i] + 1;
        if( m_rateCount[i] >= rate.toInt())
            m_rateCount[i] = 0;

        delay2();

    } //End track loop

    m_realPositionCount += 1;
    if(m_realPositionCount >= iRealPos )
        m_realPositionCount = 0;

    if( m_nextRoot != NULL && (m_realPositionCount % iHalfPos  == 0 ))
    {
        m_root = m_nextRoot;
        m_nextRoot = NULL;
        m_realPositionCount = 0;
        emit setQueueLabel("");
    }
}

void PlayClass::stopPlaying()
{
    m_stop = true;
}

void PlayClass::setBPM(int bpm)
{
    m_BPM = bpm;
}

