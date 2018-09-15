#ifndef MESSAGE_H
#define MESSAGE_H

#include "stdio.h"

class Message
{
public:
    explicit Message(unsigned long msg = 0);
    void setMsg(unsigned long msg);
    char * channelText();
    char * typeText();
    char * valueOneText();
    char * valueTwoText();
    void systemByteText(unsigned char status);

private:
    unsigned char m_status;
    unsigned char m_data1;
    unsigned char m_data2;
    unsigned char m_data3;
    char m_buf[32];
};

#endif // MESSAGE_H
