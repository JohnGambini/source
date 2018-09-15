#ifndef HEADER_H
#define HEADER_H

#include "clockdisplay.h"
#include "transportcontrol.h"
#include "timesignaturebuttons.h"

#include <QWidget>
#include <QPushButton>

class header : public QWidget
{
    Q_OBJECT
public:
    explicit header(QWidget *parent = 0);
    void paintEvent(QPaintEvent *);

signals:
    void bpmChanged(int);
    void tsChanged(int);
    void buttonPressed(int);

public slots:
    void setBPM(int value);
    void setTS(int value);
    void setStop();


private:
    ClockDisplay * m_clockDisplay;
    TransportControl * m_transportControl;
    TimeSignatureButtons * m_timeSignatureButtons;
};

#endif // HEADER_H
