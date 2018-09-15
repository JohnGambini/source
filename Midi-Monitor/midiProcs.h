#ifndef MIDIPROCS_H
#define MIDIPROCS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "pm_common/portmidi.h"
#include "pm_common/pmutil.h"
#include "porttime/porttime.h"

enum midi_messages
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

enum midi_controllers
{
    COARSE_BANK_SELECT      = 0,
    COARSE_MODULATION_WHL   = 1,
    COARSE_BREATH_CONTLLR   = 2,
    COARSE_FOOT_PEDAL       = 4,
    COARSE_PORTAMENTO_TIME  = 5,
    COARSE_DATA_ENTRY       = 6,
    COARSE_VOLUME           = 7,
    COARSE_BALANCE          = 8,
    COARSE_PAN              = 10,
    COARSE_EXPRESSION       = 11,
    COARSE_EFFECT_1         = 12,
    COARSE_EFFECT_2         = 13,
    GEN_PURPOSE_SLIDER_1    = 16,
    GEN_PURPOSE_SLIDER_2    = 17,
    GEN_PURPOSE_SLIDER_3    = 18,
    GEN_PURPOSE_SLIDER_4    = 19,
    FINE_BANK_SELECT        = 32,
    FINE_MODULATION_WHL     = 33,
    FINE_BREATH_CONTLLR     = 34,
    FINE_FOOT_PEDAL         = 36,
    FINE_PORTAMENTO_TIME    = 37,
    FINE_DATA_ENTRY         = 38,
    FINE_VOLUME             = 39,
    FINE_BALANCE            = 40,
    FINE_PAN_POSITION       = 42,
    FINE_EXPRESSION         = 43,
    FINE_EFFECT_1           = 44,
    FINE_EFFECT_2           = 45,
    HOLD_PEDAL              = 64,
    SND_VARIATION           = 70,
    SND_TIMBRE              = 71,
    SND_RELEASE_TIME        = 72, // VCA's envelope release time
    SND_ATTACK_TIME         = 73, // VCA's envelope attack time
    SND_BRIGHTNESS          = 74, // VCF's Cutoff Frequency
    SND_CONTROL_6           = 75,
    SND_CONTROL_7           = 76,
    SND_CONTROL_8           = 77,
    SND_CONTROL_9           = 78,
    SND_CONTROL_10          = 79,
    GEN_PURPOSE_BUTTON_1    = 80,
    GEN_PURPOSE_BUTTON_2    = 81,
    GEN_PURPOSE_BUTTON_3    = 82,
    GEN_PURPOSE_BUTTON_4    = 83,
    EFFECTS_LEVEL           = 91,
    TREMELO_LEVEL           = 92,
    CHORUS_LEVEL            = 93,
    CELESTE_LEVEL           = 94,
    PHASE_LEVEL             = 95,
    DATA_BUTTON_INC         = 96,
    DATA_BUTTON_DEC         = 97,
    FINE_NRPN               = 98,
    COARSE_NRPN             = 99,
    FINE_RPN                = 100,
    COARSE_RPN              = 101,
    ALL_SOUND_OFF           = 120,
    ALL_CONTROLLERS_OFF     = 121,
    LOCAL_KYBD_ON_OFF       = 122, // 0 to 63 is off
    ALL_NOTES_OFF           = 123, // Keys still held down should not turn off
    OMNI_MODE_OFF           = 124,
    OMNI_MODE_ON            = 125,
    MONO_OPERATION          = 126,
    POLY_OPERATION          = 127
};

struct PortMsg
{
    int32_t port;
    int32_t msg;
};

struct TempTrackValues
{
    unsigned long Patch;
    unsigned long PanValue;
    unsigned long NoteValue;
    unsigned long NotePos;
    unsigned long MuteValue;
    unsigned long Velocity;
    unsigned long timeStamp;
    unsigned long position;
};

typedef PmTimestamp (PmPortCallback)(void * userData);

extern PmPortCallback midiport_time_proc;
extern PtCallback process_midi;

extern float tempo;
extern int send_start_stop;
extern bool midi_proc_active;
extern bool send_mmc;
extern bool send_mtc;
extern bool midi_pause;
extern bool clock_is_running;

extern PmTimestamp current_timestamp;
extern PmTimestamp clock_start_time;
extern PmTimestamp next_clock_time;
extern PmTimestamp last_time_index;

extern PortMidiStream * midi_in_ports[10];
extern PortMidiStream * midi_out_ports[10];

/* shared queues */
extern PmQueue *midi_to_app;
extern PmQueue *app_to_midi;

extern TempTrackValues lastTrackValue[16];
extern TempTrackValues nextNoteValues[16];


#ifdef __cplusplus
}
#endif



#endif // MIDIPROCS_H
