#include "clockdisplay.h"
#include "stdio.h"

#include <QBoxLayout>
#include <QFontDatabase>

ClockDisplay::ClockDisplay(QWidget *parent) :
    QFrame(parent)
{
    int ret = 0;
    setObjectName("ClockDisplay");

    QFontDatabase aDatabase;
    ret = aDatabase.addApplicationFont(":/Sequencer/fonts/LCD-BOLD.TTF");
    QStringList fList = aDatabase.families();

    QFont aFont;
    aFont.setFamily("LCD");
    aFont.setPointSize(8);

    QFont bFont;
    bFont.setFamily("LCD");
    bFont.setPointSize(16);

    QFont cFont;
    cFont.setFamily("LCD");
    cFont.setPointSize(8);

    QBoxLayout * layout = new QBoxLayout(QBoxLayout::LeftToRight, this);
    layout->setMargin(0);
    layout->setSpacing(2);

    QLabel * timeLabel = new QLabel;
    //timeLabel->setStyleSheet("color:rgb(64,64,160)");
    aFont.setPointSize(10);
    timeLabel->setFont(aFont);
    timeLabel->setText("Time");
    timeLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    layout->addWidget(timeLabel);

    m_clockDisplay = new QLabel;
    m_clockDisplay->setFont(bFont);
    layout->addWidget(m_clockDisplay);

    QLabel * framesLabel = new QLabel;
    cFont.setPointSize(10);
    framesLabel->setFont(cFont);
    framesLabel->setText("Frames");
    framesLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    layout->addWidget(framesLabel);

    layout->addWidget(new QWidget);
    setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::Fixed));

    setLayout(layout);

}

void ClockDisplay::setClock(unsigned long currTime, unsigned long frameCount)
{
    long hundreds   = (long) (currTime / 10) % 100;
    long seconds    = (long) ((currTime / (1000)) % 60);
    long minutes    = (long) ((currTime / (60000)) % 60);
    long hours      = (long) ((currTime / (3600000)) % 24);

    char buff[64];
    memset(buff,64,1);
    sprintf(buff," %02ld:%02ld:%02ld:%02ld   %04ld ", hours, minutes, seconds, hundreds, frameCount );
    m_clockDisplay->setText(buff);
    update();
}
