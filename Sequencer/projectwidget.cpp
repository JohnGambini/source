#include "projectwidget.h"
#include "datamodel.h"

#include <QHeaderView>
#include <QContextMenuEvent>

ProjectWidget::ProjectWidget(QWidget *parent) :
    QTableView(parent)
{
    m_editWidget = 0;

    QHeaderView * vhv = verticalHeader();
    vhv->setHidden(true);

    /*
    actInsertChild = new QAction(this);
    actInsertChild->setObjectName(QString::fromUtf8("actInsertChild"));
    //QIcon icon;
    //icon.addFile(QString::fromUtf8(":/actions/actionNew.png"), QSize(), QIcon::Normal, QIcon::Off);
    //actAddChild->setIcon(icon);
    actInsertChild->setText(tr("Insert Child", "Insert Child", 0));
    ///actAddChild->setShortcut(tr("MainWindow", "Ctrl+N", 0));

    connect(actInsertRow, SIGNAL(triggered()), this, SLOT(insertRow()));
    connect(actInsertColumn, SIGNAL(triggered()), this, SLOT(insertColumn()));
    connect(actRemoveRow, SIGNAL(triggered()), this, SLOT(removeRow()));
    connect(actRemoveColumn, SIGNAL(triggered()), this, SLOT(removeColumn()));
    connect(actInsertChild, SIGNAL(triggered()), this, SLOT(insertChild()));

    m_menu = new QMenu(this);
    m_menu->addAction(actInsertRow);
    m_menu->addAction(actInsertColumn);
    m_menu->addAction(actRemoveRow);
    m_menu->addAction(actRemoveColumn);
    m_menu->addAction(actInsertChild);

    connect(m_menu, SIGNAL(aboutToShow()), this, SLOT(updateActions()));
    */
}

bool ProjectWidget::insertSequence()
{
    QAbstractItemModel * mdl = model();
    QModelIndex index = selectionModel()->currentIndex();
    int columnCount = mdl->columnCount(index.parent());

    if (!mdl->insertRow(index.row(), index.parent()))
        return false;

    int currRow = index.row();
    if( currRow < 0 ) currRow = 0;

    for (int column = 0; column < columnCount; ++column)
    {
        QModelIndex newRow = mdl->index(currRow, column, index.parent());
        mdl->setData(newRow, QVariant("[No data]"), Qt::EditRole);
    }

    selectionModel()->setCurrentIndex(mdl->index(currRow, 0, index.parent()),
                                            QItemSelectionModel::ClearAndSelect);

    return true;
}

bool ProjectWidget::addSequence()
{
    QAbstractItemModel * mdl = model();
    QModelIndex index = selectionModel()->currentIndex();
    int columnCount = mdl->columnCount(index.parent());

    if (!mdl->insertRow(index.row()+1, index.parent()))
        return false;

    int currRow = index.row()+1;
    if( currRow < 0 ) currRow = 0;

    for (int column = 0; column < columnCount; ++column)
    {
        QModelIndex newRow = mdl->index(currRow, column, index.parent());
        mdl->setData(newRow, QVariant("[New Sequence]"), Qt::EditRole);
    }

    selectionModel()->setCurrentIndex(mdl->index(currRow, 0, index.parent()),
                                            QItemSelectionModel::ClearAndSelect);

    return true;
}

bool ProjectWidget::delSelected()
{
    QAbstractItemModel * mdl = model();
    QModelIndex index = selectionModel()->currentIndex();
    gnaTreeItem * delItem = static_cast<gnaTreeItem *>(index.internalPointer());

    int currRow = index.row();
    if( currRow < 0 )
        return false;

    emit itemDeleted(delItem);

    if (!mdl->removeRow(index.row(),index.parent()))
        return false;

    return true;
}

void ProjectWidget::setRoot(gnaTreeItem * root)
{
    DataModel * mdl = new DataModel(root);
    QAbstractItemModel * oldMdl = model();
    setModel(mdl);
    connect(mdl,SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &)),
            this, SIGNAL(selectedDataChanged( const QModelIndex &, const QModelIndex &)));
    delete oldMdl;
}

void ProjectWidget::selectionChanged ( const QItemSelection & selected, const QItemSelection & deselected )
{
    gnaTreeItem * item = NULL;
    if( selectionModel()->hasSelection())
    {
        QModelIndex index = selectionModel()->currentIndex();
        item = static_cast<gnaTreeItem*>(index.internalPointer());
        emit selectedItemChanged(item);
    }
}

void ProjectWidget::contextMenuEvent(QContextMenuEvent *event)
{
    if(selectionModel()->hasSelection())
    {
        QAction * actCopySequence = new QAction(this);
        actCopySequence->setObjectName(QString::fromUtf8("actCopySequence"));
        actCopySequence->setText(tr("Copy Sequence", "Copy Sequence", 0));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/Sequencer/icons/copy.png"), QSize(), QIcon::Normal, QIcon::Off);
        actCopySequence->setIcon(icon);

        connect(actCopySequence, SIGNAL(triggered()), this, SLOT(copySequence()));

        QMenu * contextMenu = new QMenu(this);
        contextMenu->addAction(actCopySequence);

        contextMenu->exec(event->globalPos());
    }
}

void ProjectWidget::copySequence()
{
    QAbstractItemModel * mdl = model();
    QModelIndex index = selectionModel()->currentIndex();
    gnaTreeItem * item = static_cast<gnaTreeItem*>(index.internalPointer());

    int rowCount = mdl->rowCount(index.parent());

    if (!mdl->insertRow(rowCount, index.parent()))
        return;

    QModelIndex indexCopy = mdl->index(rowCount,0);

    gnaTreeItem * newItem = static_cast<gnaTreeItem *>(indexCopy.internalPointer());
    *newItem = *item;

    selectionModel()->setCurrentIndex(mdl->index(rowCount, 0, index.parent()),
                                            QItemSelectionModel::ClearAndSelect);

    emit dataChanged();

}

void ProjectWidget::resizeEvent ( QResizeEvent * event )
{
    QAbstractItemModel * mdl = model();
    QRect rect = geometry();
    for( int column = 0; column < mdl->columnCount(); column++)
    {
        setColumnWidth(column,((rect.width()-horizontalOffset())/mdl->columnCount()-4));
    }
}
