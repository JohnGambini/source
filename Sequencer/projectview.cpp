#include "projectview.h"
#include "projectheader.h"
#include "projectwidget.h"

#include <QHeaderView>

ProjectView::ProjectView(QAbstractItemModel * dataModel, QWidget * parent) :
    QWidget( parent )
{
    QVBoxLayout * layout = new QVBoxLayout(this);
    layout->setMargin(0);
    layout->setSpacing(0);

    m_projectHeader = new ProjectHeader;
    layout->setMenuBar(m_projectHeader);
    m_projectWidget = new ProjectWidget;
    layout->addWidget(m_projectWidget);
    m_projectWidget->setModel(dataModel);

    setLayout(layout);

    for( int i = 0; i < dataModel->columnCount(); i++)
    {
        QVariant header = dataModel->headerData(i,Qt::Horizontal,Qt::DisplayRole);
        dataModel->setHeaderData(i,Qt::Horizontal,header);
    }

    connect(m_projectHeader,SIGNAL(buttonPressed(int)), this, SLOT(headerButtonPressed(int)));
    connect(m_projectWidget,SIGNAL(dataChanged()), this, SIGNAL(dataChanged()));

    connect(m_projectWidget, SIGNAL(selectedItemChanged(gnaTreeItem *)), this, SLOT(selectedItemChanged(gnaTreeItem *)));
    connect(m_projectWidget, SIGNAL(selectedItemChanged(gnaTreeItem *)), this, SIGNAL(newRoot(gnaTreeItem *)));
    connect(m_projectWidget, SIGNAL(itemDeleted(gnaTreeItem *)), this, SIGNAL(itemDeleted(gnaTreeItem *)));
    connect(m_projectWidget,SIGNAL(selectedDataChanged(const QModelIndex &,const QModelIndex &)),
            this,SLOT(selectedDataChanged(const QModelIndex &,const QModelIndex &)));

    updateActions();
}

void ProjectView::updateActions()
{
    if(m_projectWidget->selectionModel()->hasSelection())
        m_projectHeader->delEnabled(true);
    else
        m_projectHeader->delEnabled(false);
}

void ProjectView::setRoot(gnaTreeItem * root)
{
    m_projectWidget->setRoot(root);
}

void ProjectView::selectedItemChanged(gnaTreeItem * item)
{
    updateActions();
}

void ProjectView::selectedDataChanged(const QModelIndex & topLeft,const QModelIndex & bottomRight)
{
    emit dataChanged();
}

void ProjectView::headerButtonPressed(int id)
{
    switch(id)
    {
    case 0: //Handled in MainWindow
        emit newDocument();
        break;
    case 1: //Handled in MainWindow
        emit openDocument();
        break;
    case 2: //Handled in MainWindow
        emit saveDocument();
        break;
    case 3:
        if(m_projectWidget->addSequence())
            emit dataChanged();
        break;
    case 4:
        if(m_projectWidget->delSelected())
            emit dataChanged();
        break;
    default:
        break;
    }

}

