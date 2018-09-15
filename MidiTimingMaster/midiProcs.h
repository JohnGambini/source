#ifndef MIDIPROCS_H
#define MIDIPROCS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "pm_common/portmidi.h"
#include "porttime/porttime.h"

enum
  {
    NOTE_OFF          =0x80,
    NOTE_ON           =0x90,
    POLY_PRESSURE     =0xa0,
    CONTROL_CHANGE    =0xb0,
    PROGRAM_CHANGE    =0xc0,
    CHANNEL_PRESSURE  =0xd0,
    PITCH_BEND    	  =0xe0,
    SYSEX_START       =0xf0,
    MIDI_Q_FRAME      =0xf1,
    SONG_POSITION     =0xf2,
    SONG_SELECT       =0xf3,
    TUNE_REQUEST      =0xf6,
    SYSEX_END         =0xf7,
    MIDI_BEAT_CLOCK   =0xf8,
    MEASURE_END       =0xf9,
    MIDI_START        =0xfa,
    MIDI_CONTINUE     =0xfb,
    MIDI_STOP         =0xfc,
    ACTIVE_SENSE      =0xfe,
    META_EVENT        =0xff
  };

struct PortMsg
{
    int32_t port;
    int32_t msg;
};

typedef PmTimestamp (PmPortCallback)(void * userData);

extern PmPortCallback midiport_time_proc;
extern PtCallback timer_poll;
extern PtCallback process_midi;

extern int send_start_stop;
extern bool midi_proc_active;
extern bool send_ssp;
extern bool clock_is_running;

extern PmTimestamp current_timestamp;
extern PmTimestamp clock_start_time;
extern PmTimestamp next_clock_time;
extern PmTimestamp last_time_index;

extern PortMidiStream * midi_in_ports[10];
extern PortMidiStream * midi_out_ports[10];

/* to determine ms per clock:
 *    time per beat in seconds =  60 / tempo
 *    multiply by 1000 to get time per beat in ms: 60000 / tempo
 *    divide by 24 CLOCKs per beat: (60000/24) / tempo
 *    simplify: 2500 / tempo
 */
#define TEMPO_TO_CLOCK 2500.0

#ifdef __cplusplus
}
#endif

#endif // MIDIPROCS_H
