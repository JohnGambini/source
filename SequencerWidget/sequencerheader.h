#ifndef SEQUENCERHEADER_H
#define SEQUENCERHEADER_H

#include <QWidget>
#include <QBoxLayout>
#include <QComboBox>
#include <QSpinBox>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QRadioButton>

#include "TransportControl.h"

class SequencerHeader : public QWidget
{
    Q_OBJECT
public:
    explicit SequencerHeader(QWidget *parent = 0);
    void paintEvent(QPaintEvent *);
    void addWidget(QWidget * widget);
    int BPM();
    void setBPM(int val);
    void setSteps(int val);
    void setQueueLabel(QString val);
    void setStopButton();
    unsigned long m_timeIndex;

signals:
    void bpmChanged(int);
    void buttonPressed(int);
    void pauseButton();

public slots:
    void setClock(unsigned long currentTime);

private:
    TransportControl * m_transportControl;

    QBoxLayout * m_layout;
    QPushButton * m_playButton;
    QPushButton * m_stopButton;
    QSpinBox * m_bpmSB;
    QLabel * m_queLabel;
    QLabel * m_smpte;

    QRadioButton * m_steps10;
    QRadioButton * m_steps12;
    QRadioButton * m_steps14;
    QRadioButton * m_steps16;

    enum
    {
        PLAY,
        PAUSE,
        STOP
    };

};

#endif // SEQUENCERHEADER_H
