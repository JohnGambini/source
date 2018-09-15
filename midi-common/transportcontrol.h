#ifndef TRANSPORTCONTROL_H
#define TRANSPORTCONTROL_H

#include <QFrame>
#include <QPushButton>
#include <QSpinBox>
#include <QComboBox>

#include "midi-common_global.h"

class MIDICOMMONSHARED_EXPORT TransportControl : public QFrame
{
    Q_OBJECT
public:
    explicit TransportControl(QWidget *parent = 0);

    enum StyleBits
    {
        PrevButton = 0x01,
        PlayButton = 0x02,
        StopButton = 0x04,
        RecordButton = 0x08,
        NextButton = 0x10
    };

    void setStyleBits(unsigned int styleBits);

signals:
    void buttonPressed(int);

public slots:
    //Prev=0 Play = 1 Pause = 2 Stop = 3 Record = 4 Next = 5
    void buttonClicked(int);


private:

    QPushButton * m_prevButton;
    QPushButton * m_playButton;
    QPushButton * m_stopButton;
    QPushButton * m_recordButton;
    QPushButton * m_nextButton;

    int m_styleBits;

};

#endif // TRANSPORTCONTROL_H
