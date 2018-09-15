#include "timesignaturebuttons.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QRadioButton>

TimeSignatureButtons::TimeSignatureButtons(QWidget *parent) :
    QFrame(parent)
{
    QHBoxLayout * layout = new QHBoxLayout;
    layout->setMargin(0);
    layout->setSpacing(2);

    layout->addWidget(new QWidget);

    //------- Button Box ----------------
    QHBoxLayout * buttonBox = new QHBoxLayout;

    QLabel * spacer = new QLabel;
    spacer->setText("Steps:  ");
    buttonBox->addWidget(spacer);

    m_steps = new QButtonGroup(this);

    QRadioButton * steps10 = new QRadioButton;
    steps10->setText("10");
    steps10->setCheckable(true);
    m_steps->addButton(steps10,10);
    buttonBox->addWidget(steps10);

    QRadioButton * steps12 = new QRadioButton;
    steps12->setText("12");
    steps12->setCheckable(true);
    m_steps->addButton(steps12,12);
    buttonBox->addWidget(steps12);

    QRadioButton * steps14 = new QRadioButton;
    steps14->setText("14");
    steps14->setCheckable(true);
    m_steps->addButton(steps14,14);
    buttonBox->addWidget(steps14);

    QRadioButton * steps16 = new QRadioButton;
    steps16->setText("16");
    steps16->setCheckable(true);
    steps16->setChecked(true);
    m_steps->addButton(steps16,16);
    buttonBox->addWidget(steps16);

    layout->addLayout(buttonBox);
    layout->addWidget(new QWidget);

    // ----- Beats per minute -----

    QHBoxLayout * subLayout = new QHBoxLayout;
    subLayout->setMargin(0);
    subLayout->setSpacing(2);

    QLabel * bpm = new QLabel(this);
    bpm->setText("BPM:");
    bpm->setMaximumWidth(28);
    subLayout->addWidget(bpm);

    m_bpmSB = new QSpinBox(this);
    m_bpmSB->setRange(1,320);
    m_bpmSB->setValue(120);
    m_bpmSB->setMaximumWidth(44);
    m_bpmSB->setAlignment(Qt::AlignHCenter);
    subLayout->addWidget(m_bpmSB);

    layout->addLayout(subLayout);

    //layout->addWidget(new QWidget);

    setLayout(layout);

    setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::Fixed));

    connect(m_bpmSB, SIGNAL(valueChanged(int)), this, SIGNAL(bpmChanged(int)));
    connect(m_steps, SIGNAL(buttonClicked(int)), this, SIGNAL(tsChanged(int)));

}

void TimeSignatureButtons::setBPM(int value)
{
    blockSignals(true);
    m_bpmSB->setValue(value);
    blockSignals(false);
}

void TimeSignatureButtons::setTS(int value)
{
    blockSignals(true);
    QAbstractButton * button = m_steps->button(value);
    if(button) button->setChecked(true);
    blockSignals(false);
}
