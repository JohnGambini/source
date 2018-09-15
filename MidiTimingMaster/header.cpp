#include "header.h"

#include <QPainter>
#include <QLinearGradient>
#include <QBoxLayout>

header::header(QWidget *parent) :
    QWidget(parent)
{
    //setMinimumHeight(28);

    QPalette * palette = new QPalette;
    palette->setColor(QPalette::Foreground,Qt::white);
    //palette->setColor(QPalette::Background,Qt::lightGray);
    ///palette->setColor(QPalette::Button,Qt::lightGray);
    palette->setColor(QPalette::ButtonText,Qt::black);
    //palette->setColor(QPalette::)
    setPalette(*palette);

    QBoxLayout * layout = new QBoxLayout(QBoxLayout::LeftToRight, this);
    layout->setMargin(2);
    layout->setSpacing(2);

    m_clockDisplay = new ClockDisplay;
    m_clockDisplay->setClock(0,0);
    layout->addWidget(m_clockDisplay);

    m_transportControl = new TransportControl;
    m_transportControl->setStyleBits(TransportControl::NextButton|TransportControl::PlayButton|TransportControl::StopButton|TransportControl::PrevButton);
    layout->addWidget(m_transportControl);

    m_timeSignatureButtons = new TimeSignatureButtons;
    layout->addWidget(m_timeSignatureButtons);

    connect(m_transportControl, SIGNAL(buttonPressed(int)), this, SIGNAL(buttonPressed(int)));
    connect(m_timeSignatureButtons, SIGNAL(bpmChanged(int)), this, SIGNAL(bpmChanged(int)));
    connect(m_timeSignatureButtons, SIGNAL(tsChanged(int)), this, SIGNAL(tsChanged(int)));

    setLayout(layout);
}

void header::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    QLinearGradient linearGradient(0, 0, 0, 1.75);
    linearGradient.setColorAt(0.0, Qt::gray);
    linearGradient.setColorAt(0.6, QColor(88,88,100));
    linearGradient.setCoordinateMode(QGradient::ObjectBoundingMode);
    //QColor aColor(96,96,127);
    //QBrush aBrush(aColor);
    painter.setBrush(linearGradient);
    painter.drawRect(geometry());

    painter.end();

}

void header::setBPM(int value)
{
    //m_TransportControl->setBPM(value);
}

void header::setTS(int value)
{
    //m_TransportControl->setTS(value);
}

void header::setStop()
{
    //m_TransportControl->buttonClicked(3);
}
