#ifndef CLOCKDISPLAY_H
#define CLOCKDISPLAY_H

#include "midi-common_global.h"

#include <QWidget>
#include <QLabel>

class MIDICOMMONSHARED_EXPORT ClockDisplay : public QFrame
{
    Q_OBJECT
public:
    explicit ClockDisplay(QWidget *parent = 0);
    void setClock(unsigned long currTime, unsigned long frameCount);

signals:
    
public slots:

private:
    QLabel * m_clockDisplay;
    
};

#endif // CLOCKDISPLAY_H
