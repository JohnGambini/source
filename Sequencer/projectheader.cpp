#include "projectheader.h"

#include <QIcon>
#include <QLabel>
#include <QButtonGroup>

ProjectHeader::ProjectHeader(QWidget * parent): Header(parent)
{
    setMinimumHeight(25);

    QPalette* palette = new QPalette();
    palette->setColor(QPalette::Foreground,Qt::white);
    setPalette(*palette);

    QButtonGroup * sqncs = new QButtonGroup(this);

    QLabel * label = new QLabel("");
    label->setMaximumWidth(1);
    addWidget(label);

    QIcon newIcon(":/Sequencer/icons/actionNew.png");
    m_newButton = new QPushButton;
    m_newButton->setMaximumWidth(22);
    m_newButton->setMaximumHeight(20);
    m_newButton->setIcon(newIcon);
    m_newButton->setFlat(true);
    m_newButton->setToolTip("New File");
    sqncs->addButton(m_newButton, 0 );
    addWidget(m_newButton);

    QIcon openIcon(":/Sequencer/icons/actionOpen.png");
    m_openButton = new QPushButton;
    m_openButton->setMaximumWidth(22);
    m_openButton->setMaximumHeight(20);
    m_openButton->setIcon(openIcon);
    m_openButton->setFlat(true);
    m_openButton->setToolTip("Open File");
    sqncs->addButton(m_openButton, 1 );
    addWidget(m_openButton);

    QIcon saveIcon(":/Sequencer/icons/actionSave.png");
    m_saveButton = new QPushButton;
    m_saveButton->setMaximumWidth(22);
    m_saveButton->setMaximumHeight(20);
    m_saveButton->setIcon(saveIcon);
    m_saveButton->setFlat(true);
    m_saveButton->setToolTip("Save File");
    sqncs->addButton(m_saveButton, 2 );
    addWidget(m_saveButton);

    label = new QLabel("");
    addWidget(label);

    QIcon addIcon(":/Sequencer/icons/add.png");
    m_addButton = new QPushButton;
    m_addButton->setMaximumWidth(16);
    m_addButton->setMaximumHeight(16);
    m_addButton->setIcon(addIcon);
    m_addButton->setToolTip("Add Sequence");
    //m_addButton->setFlat(true);
    sqncs->addButton(m_addButton, 3 );
    addWidget(m_addButton);

    QIcon delIcon(":/Sequencer/icons/delete.png");
    m_delButton = new QPushButton;
    m_delButton->setMaximumWidth(16);
    m_delButton->setMaximumHeight(16);
    m_delButton->setIcon(delIcon);
    m_delButton->setToolTip("Delete Sequence");
    //m_delButton->setFlat(true);
    sqncs->addButton(m_delButton, 4 );
    addWidget(m_delButton);

    label = new QLabel("");
    label->setMaximumWidth(1);
    addWidget(label);

    connect(sqncs,SIGNAL(buttonClicked(int)), this, SIGNAL(buttonPressed(int)));

}

void ProjectHeader::addEnabled(bool value)
{
    m_addButton->setEnabled(value);
}

void ProjectHeader::delEnabled(bool value)
{
    m_delButton->setEnabled(value);
}
