#ifndef MIDI_H
#define MIDI_H

//
// MIDI Status bytes
//

  enum
    {
      NOTE_OFF          =0x80,
      NOTE_ON           =0x90,
      POLY_PRESSURE     =0xa0,
      CONTROL_CHANGE    =0xb0,
      PROGRAM_CHANGE    =0xc0,
      CHANNEL_PRESSURE  =0xd0,
      PITCH_BEND    	=0xe0,
      SYSEX_START       =0xf0,
      MTC               =0xf1,
      SONG_POSITION     =0xf2,
      SONG_SELECT       =0xf3,
      TUNE_REQUEST      =0xf6,
      SYSEX_END         =0xf7,
      MIDI_TIME_CLOCK   =0xf8,
      MEASURE_END       =0xf9,
      MIDI_START        =0xfa,
      MIDI_CONTINUE     =0xfb,
      MIDI_STOP         =0xfc,
      ACTIVE_SENSE      =0xfe,
      META_EVENT        =0xff
    };

#endif // MIDI_H
