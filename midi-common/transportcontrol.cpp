#include "transportcontrol.h"

#include <QHBoxLayout>
#include <QButtonGroup>
#include <QLabel>

TransportControl::TransportControl(QWidget *parent) :
    QFrame(parent)
{
    m_styleBits = NextButton | PlayButton | StopButton | RecordButton | PrevButton;

    QHBoxLayout * layout = new QHBoxLayout;
    layout->setMargin(0);
    layout->setSpacing(0);

    QWidget * spacer = new QWidget;
    layout->addWidget(spacer);

    //Start and Stop buttons
    QButtonGroup * realtime = new QButtonGroup(this);
    realtime->setExclusive(false);

    m_prevButton = new QPushButton;
    QIcon prevIcon(":/Sequencer/icons/prev.png");
    m_prevButton->setIcon(prevIcon);
    //m_prevButton->setMaximumHeight(18);
    m_prevButton->setMaximumWidth(128);
    m_prevButton->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::Fixed));
    m_prevButton->setToolTip("Previous Key Frame");
    realtime->addButton(m_prevButton,0);
    layout->addWidget(m_prevButton);

    m_playButton = new QPushButton;
    QIcon PlayIcon(":/Sequencer/icons/PausePlay.png");
    QSize iconSize = PlayIcon.actualSize(QSize(32,16));
    m_playButton->setIconSize(iconSize);
    m_playButton->setIcon(PlayIcon);
    //m_playButton->setMaximumHeight(18);
    m_playButton->setMaximumWidth(128);
    m_playButton->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::Fixed));
    m_playButton->setToolTip("Play / Pause");
    m_playButton->setCheckable(true);
    realtime->addButton(m_playButton,1);
    layout->addWidget(m_playButton);

    m_stopButton = new QPushButton;
    QIcon StopIcon(":/Sequencer/icons/Stop.png");
    m_stopButton->setIcon(StopIcon);
    //m_stopButton->setMaximumHeight(18);
    m_stopButton->setMaximumWidth(128);
    m_stopButton->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::Fixed));
    m_stopButton->setToolTip("Stop");
    realtime->addButton(m_stopButton,3);
    layout->addWidget(m_stopButton);

    m_recordButton = new QPushButton;
    QIcon recordIcon(":/Sequencer/icons/Record.png");
    m_recordButton->setIcon(recordIcon);
    //m_recordButton->setMaximumHeight(18);
    m_recordButton->setMaximumWidth(128);
    m_recordButton->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::Fixed));
    m_recordButton->setToolTip("Record");
    m_recordButton->setCheckable(true);
    realtime->addButton(m_recordButton,4);
    layout->addWidget(m_recordButton);

    m_nextButton = new QPushButton;
    QIcon nextIcon(":/Sequencer/icons/next.png");
    m_nextButton->setIcon(nextIcon);
    //m_nextButton->setMaximumHeight(18);
    m_nextButton->setMaximumWidth(128);
    m_nextButton->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::Fixed));
    m_nextButton->setToolTip("Next Key Frame");
    realtime->addButton(m_nextButton,0);
    layout->addWidget(m_nextButton);

    spacer = new QWidget;
    layout->addWidget(spacer);

    //------------------------------------

    spacer = new QWidget;
    spacer->setMaximumWidth(32);
    layout->addWidget(spacer);

    setLayout(layout);
    setSizePolicy(QSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed));

    connect(realtime, SIGNAL(buttonClicked(int)), this, SLOT(buttonClicked(int)));

}

void TransportControl::buttonClicked(int id)
{
    switch(id)
    {
    case 0: //Prev
        emit(buttonPressed(0));
        break;

    case 1: //Play
        //m_bpmSB->setEnabled(false);
        if(m_playButton->isChecked())
        {
            emit(buttonPressed(1));
            m_prevButton->setEnabled(false);
            m_nextButton->setEnabled(false);
        }
        else
        {
            //m_bpmSB->setEnabled(true);
            m_recordButton->setChecked(false);
            emit(buttonPressed(2));
            m_prevButton->setEnabled(true);
            m_nextButton->setEnabled(true);
        }
        m_stopButton->setChecked(false);
        break;

    case 2: //Pause
        break;

    case 3: //Stop
        //m_bpmSB->setEnabled(true);
        m_playButton->setChecked(false);
        m_recordButton->setChecked(false);
        emit(buttonPressed(3));
        m_prevButton->setEnabled(true);
        m_nextButton->setEnabled(true);
        break;

    case 4: //Record
        m_playButton->setChecked(true);
        m_prevButton->setEnabled(false);
        m_nextButton->setEnabled(false);
        emit(buttonPressed(4));
        break;

    case 5: //Next
        emit(buttonPressed(5));
        break;

    default:
        //emit(buttonPressed(id));
        break;
    }

}

void TransportControl::setStyleBits(unsigned int styleBits)
{
    if(styleBits & PrevButton)
        m_prevButton->show();
    else
    {
        m_prevButton->hide();
    }

    if(styleBits & PlayButton)
        m_playButton->show();
    else
        m_playButton->hide();

    if(styleBits & StopButton)
        m_stopButton->show();
    else
        m_stopButton->hide();

    if(styleBits & RecordButton)
        m_recordButton->show();
    else
        m_recordButton->hide();

    if(styleBits & NextButton)
        m_nextButton->show();
    else
        m_nextButton->hide();

    update();

}
