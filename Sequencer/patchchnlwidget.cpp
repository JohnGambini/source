#include "patchchnlwidget.h"

#include <QBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QStyleFactory>

PatchChnlWidget::PatchChnlWidget(QWidget *parent) :
    QWidget(parent)
{
    m_root = NULL;
    m_rate = 2;
    m_noteOffset = 21;

    setStyleSheet("QLabel { color:white }");

    QGridLayout * layout = new QGridLayout(this);
    //layout->setSpacing(0);
    //layout->setMargin(0);

    QVBoxLayout * vLayout;

    vLayout = new QVBoxLayout;
    vLayout->setMargin(0);
    vLayout->setSpacing(1);
    m_patchSpinBox = new QComboBox;
    vLayout->addWidget(m_patchSpinBox);
    addPatchList(m_patchSpinBox);

    QLabel * patch = new QLabel;
    patch->setText("Patch");
    patch->setAlignment(Qt::AlignCenter);
    vLayout->addWidget(patch);

    layout->addLayout(vLayout,0,0,1,1);

    QHBoxLayout * hLayout = new QHBoxLayout;
    vLayout = new QVBoxLayout;
    hLayout->setMargin(0);
    //hLayout->setSpacing(0);
    m_rateButton = new QPushButton;
    m_rateButton->setText("2x");
    vLayout->addWidget(m_rateButton);
    vLayout->addWidget(new QLabel);
    hLayout->addLayout(vLayout);

    vLayout = new QVBoxLayout;
    m_muteButton = new QPushButton;
    m_muteButton->setText("Mute");
    m_muteButton->setCheckable(true);
    vLayout->addWidget(m_muteButton);
    vLayout->addWidget(new QLabel);
    hLayout->addLayout(vLayout);

    layout->addLayout(hLayout,1,0,1,1);

    vLayout = new QVBoxLayout;
    vLayout->setMargin(0);
    vLayout->setSpacing(1);
    m_portComboBox = new QComboBox;
    vLayout->addWidget(m_portComboBox);

    QLabel * portLabel = new QLabel;
    portLabel->setText("MIDI Out Port");
    portLabel->setAlignment(Qt::AlignCenter);
    vLayout->addWidget(portLabel);

    layout->addLayout(vLayout,0,1,1,1);

    //----------- Channel, Note --------------------
    hLayout = new QHBoxLayout;
    hLayout->setSpacing(8);
    hLayout->setMargin(0);

    vLayout = new QVBoxLayout;
    vLayout->setMargin(0);
    vLayout->setSpacing(1);
    m_chnlSpinBox = new QSpinBox;
    vLayout->addWidget(m_chnlSpinBox);
    m_chnlSpinBox->setMaximum(16);
    m_chnlSpinBox->setMinimum(1);
    QLabel * chnl = new QLabel;
    chnl->setText("Chnl");
    chnl->setAlignment(Qt::AlignCenter);
    vLayout->addWidget(chnl);
    hLayout->addLayout(vLayout);

    vLayout = new QVBoxLayout;
    vLayout->setMargin(0);
    vLayout->setSpacing(1);
    m_noteLabels = new QPushButton;
    m_noteLabels->setText("Notes");
    m_noteLabels->setCheckable(true);
    chnl = new QLabel;
    //chnl->setText("Chnl");
    chnl->setAlignment(Qt::AlignCenter);
    vLayout->addWidget(m_noteLabels);
    vLayout->addWidget(chnl);
    hLayout->addLayout(vLayout);

    layout->addLayout(hLayout,1,1,1,1);

    //------------ Volume -------------------------

    vLayout = new QVBoxLayout;
    vLayout->setSpacing(0);
    vLayout->setMargin(0);
    m_volumneSlider = new QSlider(Qt::Vertical);
    m_volumneSlider->setMinimum(1);
    m_volumneSlider->setMaximum(128);
    m_volumneSlider->setSliderPosition(128);
    m_volumneSlider->setStyle(QStyleFactory::create("cleanLooks"));
    m_volumneSlider->setTickPosition(QSlider::TicksAbove);
    vLayout->addWidget(m_volumneSlider,2,Qt::AlignHCenter);

    QLabel * volumeLabel = new QLabel;
    volumeLabel->setText("Volume");
    volumeLabel->setAlignment(Qt::AlignHCenter);
    volumeLabel->setStyleSheet("background-color:transparent");
    vLayout->addWidget(volumeLabel);
    vLayout->setAlignment(Qt::AlignHCenter);

    layout->addLayout(vLayout,0,2,2,1);

    connect(m_patchSpinBox,SIGNAL(currentIndexChanged(int)),this,SLOT(patchValueChanged(int)));
    connect(m_chnlSpinBox,SIGNAL(valueChanged(int)),this,SLOT(channelValueChanged(int)));
    connect(m_rateButton,SIGNAL(clicked(bool)), this, SLOT(rateValueChanged(bool)));
    connect(m_muteButton,SIGNAL(clicked(bool)), this, SLOT(muteValueChanged(bool)));
    connect(m_portComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(portValueChanged(int)));
    connect(m_volumneSlider,SIGNAL(valueChanged(int)),this,SLOT(volumeChanged(int)));
    connect(m_noteLabels,SIGNAL(clicked(bool)),this,SLOT(noteLabelsClicked(bool)));

    setLayout(layout);

}

void PatchChnlWidget::setRoot( gnaTreeItem * item )
{
    m_root = item;

    if( m_root == NULL)
        return;

    blockSignals(true);
    gnaVariant patch = m_root->data(0);
    gnaVariant chnl = m_root->data(1);
    gnaVariant port = m_root->data(2);
    gnaVariant rate = m_root->data(3);
    gnaVariant mute = m_root->data(4);
    gnaVariant volume = m_root->data(5);

    m_patchSpinBox->setCurrentIndex(patch.toInt());
    m_chnlSpinBox->setValue(chnl.toInt());
    m_portComboBox->setCurrentIndex(port.toInt());
    m_rate = rate.toInt();
    if(m_rate == 4)
        m_rateButton->setText("1x");
    if(m_rate == 2)
        m_rateButton->setText("2x");
    if(m_rate == 1)
        m_rateButton->setText("4x");
    if(m_rate == 0)
    {
        rate = 2;
        m_root->setData(3,rate);
        m_rate = 2;
        m_rateButton->setText("2x");
    }

    m_muteButton->setChecked(mute.toInt());
    m_volumneSlider->setValue(volume.toInt());
    blockSignals(false);
}

void PatchChnlWidget::setPortList( QStringList & deviceNames, QList<int> & portsToUse )
{
    for( int i = 0; i < portsToUse.count(); i++)
    {
        m_portComboBox->addItem(deviceNames[portsToUse[i]], QVariant(portsToUse[i]));
    }
}

void PatchChnlWidget::setChannel(int value)
{
    if( m_root)
        m_root->setData(1,value);
    //m_chnlSpinBox->setValue(value);
}

void PatchChnlWidget::resizeEvent(QResizeEvent * event)
{
    //QRect rect = m_chnlSpinBox->geometry();
    //int wdth = rect.width();
    //rect.adjust(wdth*.25,0,-(wdth*.25),0);
    //m_chnlSpinBox->setGeometry(rect);
}

void PatchChnlWidget::patchValueChanged(int value)
{
    gnaVariant data = value;

    if( m_root == NULL)
        return;

    m_root->setData(0,value);

    emit(dataChanged());
}

void PatchChnlWidget::channelValueChanged(int value)
{
    gnaVariant data = value;

    if( m_root == NULL)
        return;

    if(value == 10)
        m_noteOffset = 35;
    else
        m_noteOffset = 21;

    m_root->setData(1,value);

    //send track number and channel value
    emit(channelChange(m_root->childNumber(),value));

    emit(dataChanged());
}

void PatchChnlWidget::portValueChanged(int value)
{
    gnaVariant data = value;

    if( m_root == NULL)
        return;

    QVariant item = m_portComboBox->itemData(value);
    data = item.toInt();
    m_root->setData(2,data);

    emit(dataChanged());
}


void PatchChnlWidget::rateValueChanged(bool value)
{
    if( m_root == NULL)
        return;

    if( m_rate == 1 )
    {
        m_rate = 4;
        m_rateButton->setText("1x");
    }
    else if( m_rate == 2 )
    {
        m_rate = 1;
        m_rateButton->setText("4x");
    }
    else if( m_rate == 4 )
    {
        m_rate = 2;
        m_rateButton->setText("2x");
    }

    //Because this has to be queued when in play mode
    //bubble it up to the sequencer view
    emit(rateChange(m_root->childNumber(),m_rate));


    emit(dataChanged());

}

void PatchChnlWidget::muteValueChanged(bool value)
{
    if( m_root == NULL)
        return;

    gnaVariant data = m_root->data(4);
    if( data.toInt())
        data = (int)0;
    else
        data = 1;

    m_root->setData(4, data );

    emit(dataChanged());
}

void PatchChnlWidget::volumeChanged(int value)
{
    if( m_root == NULL )
        return;

    gnaVariant data = value;

    m_root->setData(5,data);

    emit(dataChanged());
}

void PatchChnlWidget::noteLabelsClicked(bool value)
{
    if(value == true)
    {
        m_noteLabels->setText("Drums");
    }
    else
    {
        m_noteLabels->setText("Notes");
    }
    emit noteLabelsChanged(value, m_root->childNumber());
}

void PatchChnlWidget::addPatchList(QComboBox * comboBox)
{
    QStringList patches;

    patches << "000-Piano 1" << "001-Piano 2" << "002-Piano 3" << "003-Honky Tonk" << "004-E.Piano 1" <<
               "005-E. Piano 2" << "006-Harpsicord" << "007-Clavinet" << "008-Celesta" << "009-Glockenspeil" <<
               "010-Music Box" << "011-Vibraphone" << "012-Marimba" << "013-Xylophone" << "014-Tubular Bell" <<
               "015-Santur" << "016-Organ 1" << "017-Organ 2" << "018-Organ 3" << "019-Church Organ" <<
               "020-Reed Organ" << "021-Accordian" << "022-Harmonica" << "023-Bandoneon" << "024-Nylon Str. Guitar" <<
               "025-Steel Str. Guitar" << "026-Jazz Guitar" << "027-Clean Guitar" << "028-Muted Guitar" <<
               "029-Overdrive Guitar" << "030-Distorted Guitar" << "031-Guitar Harmonics" << "032-Acoustic Bass" <<
               "033-Fingered Bass" << "034-Picked Bass" << "035-Fretless Bass" << "036-Slap Bass 1" << "037-Slap Bass 2" <<
               "038-Synth Bass 1" << "039-Synth Bass 2" << "040-Violin" << "041-Viola" << "042-Cello" <<
               "043-Contrabass" << "044-Tremolo Strings" << "045-Pizzicato Strings" << "046-Harp" << "047-Timpani" <<
               "048-Strings" << "049-Slow Strings" << "050-Synth Strings 1" << "051-Synth Strings 2" << "052-Choir Aahhs" <<
               "053-Voice Oohhs" << "054-Synth Vox" << "055-Orchastra Hit" << "056-Trumpet" << "057-Trombone" <<
               "058-Tuba" << "059-Muted Trumpet" << "060-French Horn" << "061-Brass 1" << "062-Synth Brass" << "063-Synth Brass2" <<
               "064-Soprano Sax" << "065-Alto Sax" << "066-Tenor Sax" << "067-Baritone Sax" << "068-Oboe" <<
               "069-English Horn" << "070-Basson" << "071-Clarinet" << "072-Piccalo" << "073-Flute" << "074-Recorder" <<
               "075-Pan Flute" << "076-Bottle Blow" << "077-Shakuhachi" << "078-Whistle" << "079-Ocarina" << "080-Square Wave" <<
               "081-Saw Wave" << "082-Syn. Calliope" << "083-Chiffer Lead" << "084-Charang" << "085-Solo Vox" <<
               "086-5th Saw Wave" << "087-Bass Lead" << "088-Fantasia" << "089-Warm Pad" << "090-PolySynth" << "091-Space Voice" <<
               "092-Bowed Glass" << "093-Metal Pad" << "094-Halo Pad" << "095-Sweep Pad" << "096-Ice Rain" << "097-Soundtrack" <<
               "098-Crystal" << "099-Atmosphere" << "100-Brightness" << "101-Goblin" << "102-Echo Drops" << "103-Star Theme" <<
               "104-Sitar" << "105-Banjo" << "106-Shamisen" << "107-Koto" << "108-Kalimba" << "109-Bagpipe" <<
               "110-Fiddle" << "111-Shanai" << "112-Tinkle Bell" << "113-Agogo" << "114-Steel Drums" << "115-Woodblock" <<
               "116-Taiko" << "117-Melo Tom 1" << "118-Synth Drum" << "119-Reverse Cym." << "120-Gt.FretNoise" <<
               "121-Breath Noise" << "122-Seashore" << "123-Bird" << "124-Telephone 1" << "125-Helicopter" <<
               "126-Applause" << "127-Gun Shot";

    comboBox->insertItems(0,patches);
}
