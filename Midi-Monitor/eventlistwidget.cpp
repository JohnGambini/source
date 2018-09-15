#include "eventlistwidget.h"
#include "message.h"

#include <QHeaderView>

EventListWidget::EventListWidget(QWidget *parent) :
    QTableView(parent)
{
    QHeaderView * vhv = verticalHeader();
    vhv->setHidden(true);

    //setColumnWidth(2,columnWidth(2)*2);
}

void EventListWidget::addRow(PmEvent event)
{
    char buf[128];
    memset(buf,128,sizeof(char));

    long hundreds   = (long) event.timestamp % 1000;
    long seconds    = (long) ((event.timestamp/ (1000)) % 60);
    long minutes    = (long) ((event.timestamp/ (60000)) % 60);
    long hours      = (long) ((event.timestamp/ (3600000)) % 24);

    sprintf(buf," %02ld:%02ld:%02ld:%03ld", hours, minutes, seconds, hundreds );

    QAbstractItemModel * mdl = model();
    QModelIndex index = selectionModel()->currentIndex();
    //int columnCount = mdl->columnCount(index.parent());

    int row = mdl->rowCount(QModelIndex());

    if (!mdl->insertRow(row, index.parent()))
        return;

    //int currRow = index.row()+1;
    //if( currRow < 0 ) currRow = 0;

    Message msg(event.message);

    QModelIndex newRow = mdl->index(row, 0, index.parent());
    mdl->setData(newRow, QVariant(buf), Qt::EditRole);
    newRow = mdl->index(row, 1, index.parent());
    mdl->setData(newRow, QVariant(msg.channelText()), Qt::EditRole);
    newRow = mdl->index(row, 2, index.parent());
    mdl->setData(newRow, QVariant(msg.typeText()), Qt::EditRole);
    newRow = mdl->index(row, 3, index.parent());
    mdl->setData(newRow, QVariant(msg.valueOneText()), Qt::EditRole);
    newRow = mdl->index(row, 4, index.parent());
    mdl->setData(newRow, QVariant(msg.valueTwoText()), Qt::EditRole);

    //for (int column = 0; column < columnCount; ++column)
    //{
    //    QModelIndex newRow = mdl->index(currRow, column, index.parent());
    //    mdl->setData(newRow, QVariant(buf), Qt::EditRole);
    //}

    //selectionModel()->setCurrentIndex(mdl->index(currRow, 0, index.parent()),
    //                                        QItemSelectionModel::ClearAndSelect);

}

void EventListWidget::clear()
{
    QAbstractItemModel * mdl = model();

    int rowCount = mdl->rowCount(QModelIndex());

    mdl->removeRows(0,rowCount,QModelIndex());

}
