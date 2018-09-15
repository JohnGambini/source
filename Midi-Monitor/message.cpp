#include "message.h"

#include "string.h"
#include "midiProcs.h"

Message::Message(unsigned long msg)
{
    memset(m_buf,32,sizeof(char));
    m_status = msg & 0xFF;
    m_data1 = (msg >> 8) & 0xFF;
    m_data2 = (msg >> 16) & 0xFF;
}

void Message::setMsg(unsigned long msg)
{
    m_status = msg & 0xFF;
    m_data1 = (msg >> 8) & 0xFF;
    m_data2 = (msg >> 16) & 0xFF;
}

char * Message::channelText()
{
    sprintf(m_buf,"%02d", (m_status) & 0x0F );
    return m_buf;
}

char * Message::typeText()
{
    switch(m_status & 0xF0)
    {
    case NOTE_OFF:
        sprintf(m_buf,"NOTE_OFF");
        break;
    case NOTE_ON:
        sprintf(m_buf,"NOTE_ON");
        break;
    case POLY_PRESSURE:
        sprintf(m_buf,"POLY_PRESSURE");
        break;
    case CONTROL_CHANGE:
        sprintf(m_buf,"CONTROL_CHANGE");
        break;
    case PROGRAM_CHANGE:
        sprintf(m_buf,"PROGRAM_CHANGE");
        break;
    case CHANNEL_PRESSURE:
        sprintf(m_buf,"CHANNEL_PRESSURE");
        break;
    case PITCH_BEND:
        sprintf(m_buf,"PITCH_BEND");
        break;
    case 0xF0:
        systemByteText(m_status & 0xFF);
        break;
    default:
        sprintf(m_buf,"None");
        break;
    }

    return m_buf;
}

char * Message::valueOneText()
{
    sprintf(m_buf,"%02d", m_data1 );
    return m_buf;
}

char * Message::valueTwoText()
{
    sprintf(m_buf,"%02d", m_data2 );
    return m_buf;
}

void Message::systemByteText(unsigned char status)
{
    switch(status)
    {
    case SYSEX_START:
        sprintf(m_buf,"SYSEX_START");
        break;
    case MIDI_Q_FRAME:
        sprintf(m_buf,"MIDI_Q_FRAME");
        break;
    case SONG_POSITION:
        sprintf(m_buf,"SONG_POSITION");
        break;
    case SONG_SELECT:
        sprintf(m_buf,"SONG_SELECT");
        break;
    case TUNE_REQUEST:
        sprintf(m_buf,"TUNE_REQUEST");
        break;
    case SYSEX_END:
        sprintf(m_buf,"SYSEX_END");
        break;
    case MIDI_BEAT_CLOCK:
        sprintf(m_buf,"MIDI_BEAT_CLOCK");
        break;
    case MEASURE_END:
        sprintf(m_buf,"MEASURE_END");
        break;
    case MIDI_START:
        sprintf(m_buf,"MIDI_START");
        break;
    case MIDI_CONTINUE:
        sprintf(m_buf,"MIDI_CONTINUE");
        break;
    case MIDI_STOP:
        sprintf(m_buf,"MIDI_STOP");
        break;
    case ACTIVE_SENSE:
        sprintf(m_buf,"ACTIVE_SENSE");
        break;
    case META_EVENT:
        sprintf(m_buf,"META_EVENT");
        break;
    default:
        sprintf(m_buf," ");
        break;
    }
}
