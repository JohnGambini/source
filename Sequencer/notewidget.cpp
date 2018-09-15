#include "notewidget.h"

#include <QBoxLayout>
#include <QResizeEvent>

NoteWidget::NoteWidget(QWidget *parent) :
    QWidget(parent)
{

    m_root = NULL;

    //21 - 108

    m_notes << "A0" << "Bb0" << "B0" << "C1"<< "C#1" << "D1" << "D#1" << "E1" << "F1" << "F#1" << "G1" << "G#1" <<
             "A1" << "Bb1" << "B1" << "C2"<< "C#2" << "D2" << "D#2" << "E2" << "F2" << "F#2" << "G2" << "G#2" <<
             "A2" << "Bb2" << "B2" << "C3"<< "C#3" << "D3" << "D#3" << "E3" << "F3" << "F#3" << "G3" << "G#3" <<
             "A3" << "Bb3" << "B3" << "C4"<< "C#4" << "D4" << "D#4" << "E4" << "F4" << "F#4" << "G4" << "G#4" <<
             "A4" << "Bb4" << "B4" << "C5"<< "C#5" << "D5" << "D#5" << "E5" << "F5" << "F#5" << "G5" << "G#5" <<
             "A5" << "Bb5" << "B5" << "C6"<< "C#6" << "D6" << "D#6" << "E6" << "F6" << "F#6" << "G6" << "G#6" <<
             "A6" << "Bb6" << "B6" << "C7"<< "C#7" << "D7" << "D#7" << "E7" << "F7" << "F#7" << "G7" << "G#7" <<
             "A7" << "Bb7" << "B7" << "C8";

    //35 - 81

    m_drums << "A0" << "Bb0" << "B0" << "C1"<< "C#1" << "D1" << "D#1" << "E1" << "F1" << "F#1" << "G1" << "G#1" <<
               "A1" << "Bb1" <<
             "Kick 2" << "Kick 1" << "Stick" << "Snare 1" << "Clap" << "Snare 2" << "Low Tom 2" << "Closed H.H." <<
             "Low Tom 1" << "Pedal H.H." << "Mid Tom 2" << "Open H.H" << "Mid Tom 1" << "High Tom 2" << "Crash 1" <<
             "High Tom 1" << "Ride 1" << "Chinese Cymb" << "Ride Bell" << "Tambourine" << "Splash Cymb" << "Cow Bell" <<
             "Crash 2" << "Vibra Slap" << "Ride 2" << "High Bongo" << "Low Bongo" << "Mute H. Conga" << "Open H. Conga" <<
             "Low Conga" << "High Timbale" << "Low Timbale" << "High Agogo" << "Low Agogo" << "Cabasa" << "Maracas" <<
             "Shrt Whistle" << "Lng Whistle" << "Shrt Guiro" << "Lng Guiro" << "Claves" << "Hgh Wood Blck" << "Low Wood Blck" <<
             "Mute Cuíca" << "Open Cuíca" << "Mute Trngl" << "Opn Trngl" <<
             "B5" << "C6"<< "C#6" << "D6" << "D#6" << "E6" << "F6" << "F#6" << "G6" << "G#6" <<
             "A6" << "Bb6" << "B6" << "C7"<< "C#7" << "D7" << "D#7" << "E7" << "F7" << "F#7" << "G7" << "G#7" <<
             "A7" << "Bb7" << "B7" << "C8";

    QBoxLayout * boxLayout = new QBoxLayout(QBoxLayout::TopToBottom);
    boxLayout->setMargin(2);
    //boxLayout->setSpacing(2);

    m_pushButton = new QPushButton;
    QIcon pushIcon(":/Sequencer/icons/Blank.png");
    m_pushButton->setIcon(pushIcon);
    m_pushButton->setCheckable(true);
    boxLayout->addWidget(m_pushButton);

    m_dial = new QDial;
    boxLayout->addWidget(m_dial);
    m_dial->setMaximum(87);
    m_dial->setMinimum(0);
    m_dial->setNotchesVisible(true);// Display notches around the dial

    m_noteSpinBox = new QComboBox;
    boxLayout->addWidget(m_noteSpinBox);
    m_noteSpinBox->addItems(m_notes);
    //m_noteSpinBox->setMaximum(110);
    //m_noteSpinBox->setMinimum(0);

    connect(m_noteSpinBox,SIGNAL(currentIndexChanged(int)),m_dial,SLOT(setValue(int)));// Changing the value of spinbox will change the position of dial
    connect(m_dial,SIGNAL(valueChanged(int)),m_noteSpinBox,SLOT(setCurrentIndex(int)));//Changing the position of dial will change the value of the spinbox
    connect(m_noteSpinBox,SIGNAL(currentIndexChanged(int)),this,SLOT(valueChanged(int)));// Changing the value of spinbox will change the position of dial
    connect(m_dial,SIGNAL(valueChanged(int)),this,SLOT(valueChanged(int)));//Changing the position of dial will change the value of the spinbox
    connect(m_pushButton, SIGNAL(clicked(bool)), this, SLOT(pushChecked(bool)));

    setLayout(boxLayout);
}

void NoteWidget::resizeEvent(QResizeEvent * event)
{
    /*
    QRect rect1= m_pushButton->geometry();
    int wdth = rect1.width();
    rect1.adjust(wdth*.10,0,-(wdth*.10),0);
    m_pushButton->setGeometry(rect1);

    QRect rect2 = m_noteSpinBox->geometry();
    wdth = rect2.width();
    rect2.adjust(wdth*.25,0,-(wdth*.25),0);
    m_noteSpinBox->setGeometry(rect2);
    */

}

void NoteWidget::setRoot(gnaTreeItem * item )
{
    m_root = item;

    if( m_root == NULL)
    {
        m_noteSpinBox->setCurrentIndex(0);
        QIcon pushIcon(":/Sequencer/icons/Blank.png");
        m_pushButton->setIcon(pushIcon);
        m_pushButton->setChecked(false);
        return;
    }

    gnaVariant noteMute = item->data(1);
    gnaVariant note = item->data(0);
    gnaVariant isInUse = item->data(2);
    if(!isInUse.toInt())
    {
        isInUse = 1;
        note = 39;
        item->setData(0,note);
        item->setData(2,isInUse);
        note = 39;
    }

    if( noteMute.toInt() )
    {
        QIcon pushIcon(":/Sequencer/icons/X.png");
        m_pushButton->setIcon(pushIcon);
        m_pushButton->setChecked(true);
    }
    else
    {
        QIcon pushIcon(":/Sequencer/icons/Blank.png");
        m_pushButton->setIcon(pushIcon);
        m_pushButton->setChecked(false);

    }

    m_noteSpinBox->setCurrentIndex(note.toInt());
}

int NoteWidget::note()
{
    return m_noteSpinBox->currentIndex();
}

void NoteWidget::setDrums()
{
    blockSignals(true);
    int value = m_noteSpinBox->currentIndex();
    while( m_noteSpinBox->count())
        m_noteSpinBox->removeItem(0);

    m_noteSpinBox->addItems(m_drums);
    m_noteSpinBox->setCurrentIndex(value);
    blockSignals(false);
}

void NoteWidget::setNotes()
{
    blockSignals(true);
    int value = m_noteSpinBox->currentIndex();
    while( m_noteSpinBox->count())
        m_noteSpinBox->removeItem(0);

    m_noteSpinBox->addItems(m_notes);
    m_noteSpinBox->setCurrentIndex(value);
    blockSignals(false);
}

void NoteWidget::noteOn(bool value)
{
    m_pushButton->setStyleSheet("background-color:red");

}

void NoteWidget::noteOff(bool value)
{
    m_pushButton->setStyleSheet("background-color:silver");
}

void NoteWidget::valueChanged(int value)
{
    gnaVariant data = value;

    if( m_root == NULL)
        return;

    m_root->setData(0,data);

    emit(dataChanged());
}

void NoteWidget::pushChecked(bool checked)
{
    gnaVariant data = (int)checked;
    if(m_root != NULL)
        m_root->setData(1,data);

    if( checked )
    {
        QIcon pushIcon(":/Sequencer/icons/X.png");
        m_pushButton->setIcon(pushIcon);

    }
    else
    {
        QIcon pushIcon(":/Sequencer/icons/Blank.png");
        m_pushButton->setIcon(pushIcon);

    }

}

void NoteWidget::setLabel(QString label)
{
    m_pushButton->setText(label);
}
