#ifndef TIMESIGNATUREBUTTONS_H
#define TIMESIGNATUREBUTTONS_H

#include "midi-common_global.h"

#include <QWidget>
#include <QSpinBox>
#include <QComboBox>
#include <QButtonGroup>

class MIDICOMMONSHARED_EXPORT TimeSignatureButtons : public QFrame
{
    Q_OBJECT
public:
    explicit TimeSignatureButtons(QWidget *parent = 0);
    

signals:
    void bpmChanged(int);
    void tsChanged(int);


public slots:
    void setBPM(int value);
    void setTS(int value);


private:
    QSpinBox * m_bpmSB;
    QButtonGroup * m_steps;

    
};

#endif // TIMESIGNATUREBUTTONS_H
