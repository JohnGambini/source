#include "midisettings.h"
#include "ui_midisettings.h"
#include "midiappcolumns.h"

#include <QStyleFactory>

MIDISettings::MIDISettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MIDISettings)
{

    m_styleBits = InputPorts | OutputPorts;

    ui->setupUi(this);
    m_root = NULL;
    m_errLog = NULL;

    ui->m_inMMCCheckbox->setEnabled(false);
    ui->m_inMTCCheckbox->setEnabled(false);

    ui->m_outMMCCheckbox->setEnabled(false);
    ui->m_outMTCCheckbox->setEnabled(false);
    ui->m_comboBox->setEnabled(false);

    setWindowTitle("MIDI Port Settings");

    QStringList frameRates;
    frameRates << "24 frames per second" << "25 frames per second" << "30 fps drop frame" << "30 frames per second";

    ui->m_comboBox->addItems(frameRates);

    connect(ui->m_listInputPorts,SIGNAL(itemSelectionChanged()),this,SLOT(inSelectionChanged()));
    connect(ui->m_listInputPorts,SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(inItemClicked(QListWidgetItem *)));
    connect(ui->m_listOutputPorts,SIGNAL(itemSelectionChanged()),this,SLOT(outSelectionChanged()));
    connect(ui->m_listOutputPorts,SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(outItemClicked(QListWidgetItem *)));

    connect(ui->m_inMMCCheckbox,SIGNAL(stateChanged(int)),this,SLOT(inMMCChanged(int)));
    connect(ui->m_inMTCCheckbox,SIGNAL(stateChanged(int)),this,SLOT(inMTCChanged(int)));
    connect(ui->m_outMMCCheckbox,SIGNAL(stateChanged(int)),this,SLOT(outMMCChanged(int)));
    connect(ui->m_outMTCCheckbox,SIGNAL(stateChanged(int)),this,SLOT(outMTCChanged(int)));

    connect(ui->m_comboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(fpsChanged(int)));

    m_moveBy = ui->labelOutputPorts->geometry().y() - ui->labelInputPorts->geometry().y();

}

MIDISettings::~MIDISettings()
{
    delete ui;
}

void MIDISettings::setStyleBits(unsigned int styleBits)
{
    m_styleBits = styleBits;
    if( m_styleBits & InputPorts)
    {
        ui->labelInputPorts->show();
        ui->m_listInputPorts->show();
        ui->m_inMMCCheckbox->show();
        ui->m_inMTCCheckbox->show();

    }
    else
    {
        ui->labelInputPorts->hide();
        ui->m_listInputPorts->hide();
        ui->m_inMMCCheckbox->hide();
        ui->m_inMTCCheckbox->hide();

        ui->labelOutputPorts->setGeometry(ui->labelInputPorts->geometry());
        ui->m_listOutputPorts->setGeometry(ui->m_listInputPorts->geometry());
        ui->m_outMMCCheckbox->setGeometry(ui->m_inMMCCheckbox->geometry());
        ui->m_outMTCCheckbox->setGeometry(ui->m_inMTCCheckbox->geometry());

        QRect rect = ui->m_comboBox->geometry();
        ui->m_comboBox->move(rect.x(), rect.y()- m_moveBy);

        rect = ui->buttonBox->geometry();
        ui->buttonBox->move(rect.x(), rect.y()- m_moveBy);

        QSizePolicy sP = sizePolicy();

        rect = geometry();
        rect.setBottom(rect.bottom()-m_moveBy);
        resize(rect.width(),rect.height());
    }

    if(m_styleBits & OutputPorts)
    {
        ui->labelOutputPorts->show();
        ui->m_listOutputPorts->show();
        ui->m_outMMCCheckbox->show();
        ui->m_outMTCCheckbox->show();

    }
    else
    {
        ui->labelOutputPorts->hide();
        ui->m_listOutputPorts->hide();
        ui->m_outMMCCheckbox->hide();
        ui->m_outMTCCheckbox->hide();
    }
}

void MIDISettings::setRoot(gnaTreeItem * root)
{
    m_root = root;
    if(m_root == NULL)
        return;

    gnaTreeItem * inputPorts = m_root->child(MIDI_INPORTS);
    gnaTreeItem * outputPorts = m_root->child(MIDI_OUTPORTS);

    if(inputPorts != NULL)
        for( int i = 0; i < inputPorts->childCount(); i++)
        {
            gnaTreeItem * child = inputPorts->child(i);
            gnaVariant name = child->data(0);
            gnaVariant checked = child->data(2);
            QListWidgetItem * item = new QListWidgetItem(name.toString());
            if( checked.toInt() == 1)
                item->setCheckState(Qt::Checked);
            else
                item->setCheckState(Qt::Unchecked);
            ui->m_listInputPorts->addItem(item);
        }

    if(outputPorts != NULL)
        for( int i = 0; i < outputPorts->childCount(); i++)
        {
            gnaTreeItem * child = outputPorts->child(i);
            gnaVariant name = child->data(0);
            gnaVariant checked = child->data(2);
            QListWidgetItem * item = new QListWidgetItem(name.toString());
            if( checked.toInt() == 1)
                item->setCheckState(Qt::Checked);
            else
                item->setCheckState(Qt::Unchecked);
            ui->m_listOutputPorts->addItem(item);
        }

    gnaVariant fps = m_root->data(0);
    ui->m_comboBox->setCurrentIndex(fps.toInt());
}

void MIDISettings::accept()
{
    if( m_root == NULL)
        return;

    gnaTreeItem * inputPorts = m_root->child(MIDI_INPORTS);
    gnaTreeItem * outputPorts = m_root->child(MIDI_OUTPORTS);

    gnaVariant checked;

    for( int i = 0; i < inputPorts->childCount(); i++)
    {
        gnaTreeItem * child = inputPorts->child(i);
        QListWidgetItem * item = ui->m_listInputPorts->item(i);
        if(item->checkState() == Qt::Checked)
            checked = 1;
        else
            checked = 0;
        child->setData(2,checked);
    }

    for( int i = 0; i < outputPorts->childCount(); i++)
    {
        gnaTreeItem * child = outputPorts->child(i);
        QListWidgetItem * item = ui->m_listOutputPorts->item(i);
        if(item->checkState() == Qt::Checked)
            checked = 1;
        else
            checked = 0;
        child->setData(2,checked);
    }

    QDialog::accept();
}

void MIDISettings::inSelectionChanged()
{
    QListWidgetItem * item = ui->m_listInputPorts->currentItem();
    QModelIndex index = ui->m_listInputPorts->currentIndex();
    if(index.row() < 0)
        return;
    gnaTreeItem * inputPorts = m_root->child(MIDI_INPORTS);
    gnaTreeItem * child = inputPorts->child(index.row());
    gnaVariant sendMMC = child->data(3);
    gnaVariant sendMTC = child->data(4);

    ui->m_inMMCCheckbox->setChecked(sendMMC.toInt());
    ui->m_inMTCCheckbox->setChecked(sendMTC.toInt());

    if(item->checkState() == Qt::Checked)
    {
        ui->m_inMMCCheckbox->setEnabled(true);
        ui->m_inMTCCheckbox->setEnabled(true);
    }
    else
    {
        ui->m_inMMCCheckbox->setEnabled(false);
        ui->m_inMTCCheckbox->setEnabled(false);
    }

}

void MIDISettings::outSelectionChanged()
{
    QListWidgetItem * item = ui->m_listOutputPorts->currentItem();
    QModelIndex index = ui->m_listOutputPorts->currentIndex();
    if(index.row() < 0)
        return;
    gnaTreeItem * outputPorts = m_root->child(MIDI_OUTPORTS);
    gnaTreeItem * child = outputPorts->child(index.row());
    gnaVariant sendMMC = child->data(3);
    gnaVariant sendMTC = child->data(4);

    ui->m_outMMCCheckbox->setChecked(sendMMC.toInt());
    ui->m_outMTCCheckbox->setChecked(sendMTC.toInt());

    if(item->checkState() == Qt::Checked)
    {
        ui->m_outMMCCheckbox->setEnabled(true);
        ui->m_outMTCCheckbox->setEnabled(true);
        if(sendMTC.toInt() != 0)
            ui->m_comboBox->setEnabled(true);
        else
            ui->m_comboBox->setEnabled(false);
    }
    else
    {
        ui->m_outMMCCheckbox->setEnabled(false);
        ui->m_outMTCCheckbox->setEnabled(false);
        ui->m_comboBox->setEnabled(false);
    }

}

void MIDISettings::inItemClicked(QListWidgetItem * item)
{
    QListWidgetItem * item2 = ui->m_listInputPorts->currentItem();
    QModelIndex index = ui->m_listInputPorts->currentIndex();
    if(index.row() < 0)
        return;
    gnaTreeItem * inputPorts = m_root->child(MIDI_INPORTS);
    gnaTreeItem * child = inputPorts->child(index.row());

    if(item->checkState() == Qt::Checked)
    {
        ui->m_inMMCCheckbox->setEnabled(true);
        ui->m_inMTCCheckbox->setEnabled(true);
    }
    else
    {
        ui->m_inMMCCheckbox->setEnabled(false);
        ui->m_inMTCCheckbox->setEnabled(false);
    }
}

void MIDISettings::outItemClicked(QListWidgetItem * item)
{
    bool handleFps = false;
    QListWidgetItem * item2 = ui->m_listOutputPorts->currentItem();
    QModelIndex index = ui->m_listOutputPorts->currentIndex();
    if(index.row() < 0)
        return;
    gnaTreeItem * outputPorts = m_root->child(MIDI_OUTPORTS);
    gnaTreeItem * child = outputPorts->child(index.row());
    gnaVariant sendMTC = child->data(4);
    if(item2 == item)
        handleFps = true;

    if(item->checkState() == Qt::Checked)
    {
        ui->m_outMMCCheckbox->setEnabled(true);
        ui->m_outMTCCheckbox->setEnabled(true);
        if(handleFps && sendMTC.toInt() != 0)
            ui->m_comboBox->setEnabled(true);
        else
            ui->m_comboBox->setEnabled(false);
    }
    else
    {
        ui->m_outMMCCheckbox->setEnabled(false);
        ui->m_outMTCCheckbox->setEnabled(false);
        if(handleFps)
            ui->m_comboBox->setEnabled(false);
    }
}

void MIDISettings::inMMCChanged(int value)
{
    QModelIndex index = ui->m_listInputPorts->currentIndex();
    gnaTreeItem * inputPorts = m_root->child(MIDI_INPORTS);
    gnaTreeItem * child = inputPorts->child(index.row());
    gnaVariant sendMMC = value;
    child->setData(3,sendMMC);
}

void MIDISettings::inMTCChanged(int value)
{
    QModelIndex index = ui->m_listInputPorts->currentIndex();
    gnaTreeItem * inputPorts = m_root->child(MIDI_INPORTS);
    gnaTreeItem * child = inputPorts->child(index.row());
    gnaVariant sendMTC = value;
    child->setData(4,sendMTC);
}

void MIDISettings::outMMCChanged(int value)
{
    QModelIndex index = ui->m_listOutputPorts->currentIndex();
    gnaTreeItem * inputPorts = m_root->child(MIDI_OUTPORTS);
    gnaTreeItem * child = inputPorts->child(index.row());
    gnaVariant sendMMC = value;
    child->setData(3,sendMMC);
}

void MIDISettings::outMTCChanged(int value)
{
    QModelIndex index = ui->m_listOutputPorts->currentIndex();
    gnaTreeItem * outputPorts = m_root->child(MIDI_OUTPORTS);
    gnaTreeItem * child = outputPorts->child(index.row());
    gnaVariant sendMTC = value;
    child->setData(4,sendMTC);
    if( sendMTC.toInt() != 0 )
        ui->m_comboBox->setEnabled(true);
    else
        ui->m_comboBox->setEnabled(false);
}


void MIDISettings::fpsChanged(int value)
{
    if(m_root == NULL)
        return;

    gnaVariant fps = value;
    m_root->setData(0,fps);
}
