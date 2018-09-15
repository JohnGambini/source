#include "sequencerheader.h"

#include <QIcon>
#include <QButtonGroup>

#include <QPainter>
#include <QStyle>

#include "stdio.h"

SequencerHeader::SequencerHeader(QWidget *parent) :
    QWidget(parent)
{
    m_timeIndex = 0;

    setMinimumHeight(25);

    QPalette * palette = new QPalette;
    palette->setColor(QPalette::Foreground,Qt::white);
    setPalette(*palette);

    m_layout = new QBoxLayout(QBoxLayout::LeftToRight, this);
    m_layout->setMargin(0);
    m_layout->setSpacing(2);

    QLabel * spacer = new QLabel(this);
    spacer->setMaximumWidth(spacer->width()/2);
    spacer->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::Fixed));
    m_layout->addWidget(spacer);

    m_smpte = new QLabel(this);
    m_smpte->setAlignment(Qt::AlignHCenter | Qt::AlignCenter);
    m_smpte->setStyleSheet("color:rgb(192,192,248); border-width:1px; border-style:outset; border-color:black");
    m_smpte->setMaximumHeight(18);
    m_smpte->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::Fixed));
    QFont smteFont("Lucida Console", 11);
    m_smpte->setFont(smteFont);
    m_smpte->setText(" 00:00:00:00 ");
    m_layout->addWidget(m_smpte);

    m_queLabel = new QLabel;
    m_queLabel->setAlignment(Qt::AlignHCenter | Qt::AlignCenter);
    m_queLabel->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::Fixed));
    //m_queLabel->setText("Sequence Queued...");
    m_layout->addWidget(m_queLabel);

    m_transportControl = new TransportControl;
    m_transportControl->setMaximumHeight(18);
    m_transportControl->setStyleBits(TransportControl::PlayButton | TransportControl::StopButton);
    m_layout->addWidget(m_transportControl);

    spacer = new QLabel(this);
    spacer->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::Fixed));
    m_layout->addWidget(spacer);

    //------- Button Box ----------------
    QHBoxLayout * buttonBox = new QHBoxLayout;

    spacer = new QLabel;
    spacer->setText("Steps:");
    buttonBox->addWidget(spacer);

    QButtonGroup * steps = new QButtonGroup(this);

    m_steps10 = new QRadioButton;
    m_steps10->setText("10");
    m_steps10->setCheckable(true);
    steps->addButton(m_steps10,10);
    buttonBox->addWidget(m_steps10);

    m_steps12 = new QRadioButton;
    m_steps12->setText("12");
    m_steps12->setCheckable(true);
    steps->addButton(m_steps12,12);
    buttonBox->addWidget(m_steps12);

    m_steps14 = new QRadioButton;
    m_steps14->setText("14");
    m_steps14->setCheckable(true);
    steps->addButton(m_steps14,14);
    buttonBox->addWidget(m_steps14);

    m_steps16 = new QRadioButton;
    m_steps16->setText("16");
    m_steps16->setCheckable(true);
    m_steps16->setChecked(true);
    steps->addButton(m_steps16,16);
    buttonBox->addWidget(m_steps16);

    m_layout->addLayout(buttonBox);

    spacer = new QLabel(this);
    spacer->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::Fixed));
    m_layout->addWidget(spacer);

    // ----- Beats per minute -----
    QLabel * bpm = new QLabel(this);
    bpm->setText("BPM:");
    bpm->setMaximumWidth(32);
    m_layout->addWidget(bpm);

    m_bpmSB = new QSpinBox(this);
    m_bpmSB->setRange(1,320);
    m_bpmSB->setValue(120);
    m_bpmSB->setMaximumWidth(63);
    m_bpmSB->setAlignment(Qt::AlignHCenter);
    m_layout->addWidget(m_bpmSB);

    spacer = new QLabel(this);
    spacer->setMaximumWidth(32);
    spacer->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::Fixed));
    m_layout->addWidget(spacer);

    connect(m_transportControl, SIGNAL(buttonPressed(int)), this, SIGNAL(buttonPressed(int)));
    connect(m_bpmSB, SIGNAL(valueChanged(int)), this, SIGNAL(bpmChanged(int)));
    //connect(steps, SIGNAL(buttonClicked(int)), this, SLOT(buttonClicked(int)));

}

void SequencerHeader::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    QLinearGradient linearGradient(0, 0, 0, 1.75);
    linearGradient.setColorAt(0.0, Qt::gray);
    linearGradient.setColorAt(0.6, QColor(42,42,64));
    linearGradient.setCoordinateMode(QGradient::ObjectBoundingMode);
    //QColor aColor(96,96,127);
    //QBrush aBrush(aColor);
    painter.setBrush(linearGradient);
    painter.drawRect(geometry());

    painter.end();

}

void SequencerHeader::addWidget(QWidget * widget)
{
    m_layout->addWidget(widget);
}

int SequencerHeader::BPM()
{
    return m_bpmSB->value();
}

void SequencerHeader::setBPM(int val)
{
    m_bpmSB->setValue(val);
}

void SequencerHeader::setSteps(int val)
{
    switch(val)
    {
    case 10:
        m_steps10->setChecked(true);
        break;
    case 12:
        m_steps12->setChecked(true);
        break;
    case 14:
        m_steps14->setChecked(true);
        break;
    case 16:
        m_steps16->setChecked(true);
        break;
    }

}

void SequencerHeader::setQueueLabel(QString val)
{
    m_queLabel->setText(val);
}

void SequencerHeader::setStopButton()
{
    m_stopButton->setChecked(true);
}

void SequencerHeader::setClock(unsigned long currentTime)
{
    m_timeIndex = currentTime;

    long hundreds   = (long) (currentTime / 10) % 100;
    long seconds    = (long) ((currentTime / (1000)) % 60);
    long minutes    = (long) ((currentTime / (60000)) % 60);
    long hours      = (long) ((currentTime / (3600000)) % 24);

    char buff[64];
    memset(buff,64,1);
    sprintf(buff," %02ld:%02ld:%02ld:%02ld ", hours, minutes, seconds, hundreds );
    //sprintf(buff," %d", timestamp );
    m_smpte->setText(buff);
    update();
}

