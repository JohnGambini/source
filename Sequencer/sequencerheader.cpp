#include "sequencerheader.h"

#include <QIcon>
#include <QButtonGroup>

#include <QPainter>
#include <QStyle>

SequencerHeader::SequencerHeader(QWidget *parent) :
    QWidget(parent)
{
    setMinimumHeight(25);

    QPalette * palette = new QPalette;
    palette->setColor(QPalette::Foreground,Qt::white);
    palette->setColor(QPalette::Background,Qt::lightGray);
    palette->setColor(QPalette::Button,Qt::transparent);
    palette->setColor(QPalette::ButtonText,Qt::white);
    setPalette(*palette);

    m_layout = new QBoxLayout(QBoxLayout::LeftToRight, this);
    m_layout->setMargin(0);
    m_layout->setSpacing(2);

    QLabel * spacer = new QLabel(this);
    spacer->setMaximumWidth(spacer->width()/2);
    m_layout->addWidget(spacer);

    QLabel * smteTime = new QLabel(this);
    smteTime->setAlignment(Qt::AlignHCenter | Qt::AlignCenter);
    smteTime->setStyleSheet("color:rgb(192,192,248); border-width:1px; border-style:outset; border-color:black");
    smteTime->setMaximumHeight(18);
    QFont smteFont("Lucida Console", 11);
    smteTime->setFont(smteFont);
    smteTime->setText(" 00:00:00:00 ");
    m_layout->addWidget(smteTime);

    m_queLabel = new QLabel;
    m_queLabel->setAlignment(Qt::AlignHCenter | Qt::AlignCenter);
    //m_queLabel->setText("Sequence Queued...");
    m_layout->addWidget(m_queLabel);

    //spacer = new QLabel(this);
    ///m_layout->addWidget(spacer);

    QButtonGroup * realtime = new QButtonGroup(this);

    m_playButton = new QPushButton;
    QIcon PlayIcon(":/Sequencer/icons/Play.png");
    m_playButton->setIcon(PlayIcon);
    m_playButton->setMaximumHeight(18);
    m_playButton->setMaximumWidth(128);
    m_playButton->setCheckable(true);
    realtime->addButton(m_playButton,0);
    m_layout->addWidget(m_playButton);

    m_stopButton = new QPushButton;
    QIcon StopIcon(":/Sequencer/icons/Stop.png");
    m_stopButton->setIcon(StopIcon);
    m_stopButton->setMaximumHeight(18);
    m_stopButton->setMaximumWidth(128);
    m_stopButton->setCheckable(true);
    m_stopButton->setChecked(true);
    realtime->addButton(m_stopButton,1);
    m_layout->addWidget(m_stopButton);

    m_playButton->setStyleSheet("background-color:lightgrey");
    m_stopButton->setStyleSheet("background-color:lightgrey");

    spacer = new QLabel(this);
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
    m_layout->addWidget(spacer);

    connect(realtime, SIGNAL(buttonClicked(int)), this, SLOT(buttonClicked(int)));
    connect(m_bpmSB, SIGNAL(valueChanged(int)), this, SIGNAL(bpmChanged(int)));
    connect(steps, SIGNAL(buttonClicked(int)), this, SLOT(buttonClicked(int)));

}

void SequencerHeader::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    QLinearGradient linearGradient(0, 0, 0, 2);
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

    //m_steps->setId();
}

void SequencerHeader::setQueueLabel(QString val)
{
    m_queLabel->setText(val);
}

void SequencerHeader::buttonClicked(int id)
{
    switch(id)
    {
    case 0:
        m_bpmSB->setEnabled(false);
        emit(buttonPressed(id));
        break;
    case 1:
        m_bpmSB->setEnabled(true);
        emit(buttonPressed(id));
        break;
    default:
        emit(buttonPressed(id));
        break;
    }
}

void SequencerHeader::setStopButton()
{
    m_stopButton->setChecked(true);
}

