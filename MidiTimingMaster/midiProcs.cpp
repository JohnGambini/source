#include "midiProcs.h"
#include "pm_common/portmidi.h"

PortMidiStream * midi_in_ports[10];
PortMidiStream * midi_out_ports[10];

PmTimestamp current_timestamp = 0;
PmTimestamp clock_start_time = 0;
PmTimestamp next_clock_time = 0;
PmTimestamp last_time_index = 0;

bool midi_proc_active = false;
int send_start_stop = 0;
bool send_ssp = false;
bool clock_is_running = false;
float tempo = 60.0F;
float tempo_to_frame = 2500.0;
bool is_drop_frame = false;

/* to determine ms per clock:
 *    time per beat in seconds =  60 / tempo
 *    multiply by 1000 to get time per beat in ms: 60000 / tempo
 *    divide by 24 CLOCKs per beat: (60000/24) / tempo
 *    simplify: 2500 / tempo
 */
#define TEMPO_TO_CLOCK 2500.0

/* time proc parameter for Pm_MidiOpen */
PmTimestamp midiport_time_proc(void *info)
{
    return current_timestamp;
}

void process_midi(PtTimestamp timestamp, void *userData)
{
    PmError result;
    PmEvent buffer; /* just one message at a time */

    current_timestamp++; /* update every millisecond */

    /* do nothing until initialization completes */
    if (!midi_proc_active) {
        /* this flag signals that no more midi processing will be done */
        //process_midi_exit_flag = TRUE;
        return;
    }



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
            Pm_WriteShort(midi_out_ports[0], 0, MIDI_STOP);
            if( send_ssp != true ) //2 == PAUSE as appossed to STOP
                last_time_index = timestamp - clock_start_time + last_time_index;
            //else
                //Pm_Write SSP Message
            send_ssp = false;
            clock_start_time = timestamp;
            current_timestamp = timestamp;
        }
        else
        {
            Pm_WriteShort(midi_out_ports[0], 0, MIDI_CONTINUE);
            clock_start_time = timestamp;
            next_clock_time = TEMPO_TO_CLOCK / tempo;
        }
        clock_is_running = !clock_is_running;

        send_start_stop = 0;
    }

    if(clock_is_running)
        current_timestamp = timestamp;

}
