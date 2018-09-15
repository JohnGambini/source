#ifndef MIDIAPPCOLUMNS_H
#define MIDIAPPCOLUMNS_H

enum AppSettings
{
    MIDI_INPORTS = 0,
    MIDI_OUTPORTS = 1
};


enum PortSettings
{
    PORT_NAME       = 0,
    PORTMIDI_ID     = 1,
    PORT_USED       = 2,
    PORT_MMC        = 3,
    PORT_MTC        = 4,
    MIDI_OUT_IDX    = 5,
    MIDI_IN_IDX     = 5
};

enum TimeSettings
{
    TEMPO = 1,
    STEPS = 2
};

enum ChannelSettings
{
    PATCH = 0,
    CHANNEL = 1,
    PORT_ITEM = 2,
    RATE = 3,
    MUTE = 4,
    VOLUME = 5,
    PAN = 6,
    OUT_PORT_IDX = 7
};


#endif // MIDIAPPCOLUMNS_H
