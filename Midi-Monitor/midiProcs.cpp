#include "midiProcs.h"
#include "gnaGlobal.h"

PortMidiStream * midi_in_ports[10];
PortMidiStream * midi_out_ports[10];

PmTimestamp current_timestamp = 0;
PmTimestamp clock_start_time = 0;
PmTimestamp next_clock_time = 0;
PmTimestamp last_time_index = 0;

bool midi_proc_active = false;
int send_start_stop = 0;
bool send_mmc = false;
bool send_mtc = false;
bool midi_pause = false;
bool clock_is_running = false;
float tempo = 60.0F;
float tempo_to_frame = 2500.0;
bool is_drop_frame = false;

/* shared queues */
PmQueue *midi_to_app = 0;
PmQueue *app_to_midi = 0;

TempTrackValues lastTrackValue[16];
TempTrackValues nextNoteValues[16];


/* time proc parameter for Pm_MidiOpen */
PmTimestamp midiport_time_proc(void *info)
{
    return current_timestamp;
}

void process_midi(PtTimestamp timestamp, void *userData)
{
    int result;
    PortMsg pMsg;
    int portIdx;
    PmEvent anEvent;
    PmError err;
    PmEvent buffer; /* just one message at a time */

    /* do nothing until initialization completes */
    if (!midi_proc_active)
    {
        return;
    }

    //Handle clock messages and Start and Stop Messages
    if (send_start_stop)
    {
        if (clock_is_running)
        {
            if(send_mmc)
                Pm_WriteShort(midi_out_ports[0], 0, Pm_Message(MIDI_STOP,0,0));
            if( midi_pause)
                last_time_index = timestamp - clock_start_time + last_time_index;
            //else
                //Pm_Write SSP Message
            clock_start_time = timestamp;
            current_timestamp = timestamp;
        }
        else
        {
            Pm_WriteShort(midi_out_ports[0], 0, Pm_Message(MIDI_CONTINUE,0,0));
            clock_start_time = timestamp;
            //next_clock_time = TEMPO_TO_CLOCK / tempo;
        }
        clock_is_running = !clock_is_running;

        send_start_stop = 0;
    }

    if(clock_is_running)
        current_timestamp = timestamp;

    // check for note on/off messages
    do
    {
        result = Pm_Dequeue(app_to_midi, &pMsg);
        if (result == pmGotData)
        {
            portIdx = pMsg.port;
            anEvent.message = pMsg.msg;
            anEvent.timestamp = timestamp;
            err = Pm_Write(midi_out_ports[portIdx],&anEvent, 1 );
        }
    }
    while (result == pmGotData);

    /* see if there is any midi input to process */
    do
    {
        result = Pm_Poll(midi_in_ports[0]);
        if (result)
        {
            if (Pm_Read(midi_in_ports[0], &buffer, 1) == pmBufferOverflow)
                continue;

            buffer.timestamp = timestamp;

            if ( midi_to_app != 0 )
                Pm_Enqueue(midi_to_app, &buffer);
        }
    } while (result);


}

void timer_poll(PtTimestamp timestamp, void *userData)
{
    /* do nothing until initialization completes */
    if (!midi_proc_active)
    {
        return;
    }

    if (send_start_stop)
    {
        if (clock_is_running)
        {
            if(send_mmc)
                Pm_WriteShort(midi_out_ports[0], 0, Pm_Message(MIDI_STOP,0,0));

            if(midi_pause)
                last_time_index = timestamp - clock_start_time + last_time_index;
            //else
                //Pm_Write SSP Message
            clock_start_time = timestamp;
            current_timestamp = timestamp;
        }
        else
        {
            Pm_WriteShort(midi_out_ports[0], 0, Pm_Message(MIDI_CONTINUE,0,0));
            clock_start_time = timestamp;
            //next_clock_time = TEMPO_TO_CLOCK / tempo;
        }
        clock_is_running = !clock_is_running;

        send_start_stop = 0;
    }

    if(clock_is_running)
        current_timestamp = timestamp;

}
