#include "playclass.h"

#include "midi.h"

#include "pm_common/portmidi.h"
#include "pm_common/pmutil.h"
#include "porttime/porttime.h"

/* to determine ms per clock:
 *    time per beat in seconds =  60 / tempo
 *    multiply by 1000 to get time per beat in ms: 60000 / tempo
 *    divide by 4 CLOCKs per beat: (60000/4) / tempo
 *    simplify: 15000 / tempo
 */
#define TEMPO_TO_CLOCK 15000

PtTimestamp offsetTime = 0;
unsigned long timeIndex = 125; //timeIndex is how far to start into the song plus 1/4 second lead time before time 00:00:00
bool active = false;
gnaVariant vBPM;
PmQueue *main_to_midi = NULL;
PmStream * midi_out = NULL;
PmEvent anEvent;
PmMessage aNote;
PmError err = pmNoError;
unsigned long currentTime = 0;
bool first_call = true;

unsigned long noteCount = 0;

void process_midi(PtTimestamp timestamp, void *userData)
{
    gnaTreeItem * root = static_cast<gnaTreeItem *>(userData);

    if(!active || root == NULL)
        return;

    if(first_call)
    {

        offsetTime = timestamp - offsetTime - 125;
        first_call = false;
    }

    vBPM = root->data(1);
    int bpm = vBPM.toInt();

    currentTime = timestamp - offsetTime;

    if(((currentTime) % int(TEMPO_TO_CLOCK / bpm )))
        return;

    if(noteCount % 4 )
        aNote = Pm_Message( uchar(0 | NOTE_ON),uchar(60),uchar(100));
    else
        aNote = Pm_Message( uchar(0 | NOTE_ON),uchar(60),uchar(127));
    noteCount++;
    anEvent.message = aNote;
    anEvent.timestamp = timestamp;
    err = Pm_Write(midi_out,&anEvent, 1 );

    /*
    if(err != pmNoError)
    {
        error(Pm_GetErrorText( err ));
        break;
    }
    */


}

PlayClass::PlayClass(gnaTreeItem * root, bool * pbStop, unsigned long offsetTime, QObject *parent) :
    QObject(parent)
{
    m_root = root;
    m_pbStop = pbStop;
    m_offsetTime = offsetTime;

   active = false;
}

#define OUTPUT_BUFFER_SIZE 0
#define DRIVER_INFO NULL
#define TIME_PROC ((int32_t (*)(void *)) Pt_Time)
#define TIME_INFO NULL
/* use zero latency because we want output to be immediate */
#define LATENCY 0

void PlayClass::play()
{
    int id = 0;
    PmError err = pmNoError;

    main_to_midi = Pm_QueueCreate(32, sizeof(int32_t));

    if(main_to_midi == NULL)
    {
        error("Failed to create MIDI queue.");
        emit processStopped();
        return;
    }

    Pt_Start(1, process_midi, m_root );

    Pm_Initialize();

    /* use zero latency because we want output to be immediate */
    err = Pm_OpenOutput(&midi_out,
                  id,
                  DRIVER_INFO,
                  OUTPUT_BUFFER_SIZE,
                  TIME_PROC,
                  TIME_INFO,
                  LATENCY);

    if(err != pmNoError)
    {
        error(Pm_GetErrorText( err ));
        emit processStopped();
        return;
    }

    err = Pm_Synchronize(midi_out);

    //m_offsetTime is the number of microseconds into the song where we want to start playing.
    offsetTime = m_offsetTime;

    active = true;

    while(*m_pbStop == false)
    {
        emit clockTick(Pt_Time() - offsetTime - 350 );

        Pt_Sleep(10);
    }

    active = false;

    Pt_Sleep(5);

    Pm_Close(midi_out);

    Pm_Terminate();

    Pt_Stop();

    emit processStopped();
}


