#include "datamodel.h"

DataModel::DataModel(QObject *parent) :
    QAbstractItemModel(parent)
{
    m_rootItem = 0;
}

DataModel::DataModel(gnaTreeItem * root, QObject *parent) :
    QAbstractItemModel(parent)
{
    m_rootItem = root;
}


QModelIndex DataModel::index(int row, int column, const QModelIndex &parent) const
{
    gnaTreeItem * childItem = 0;

    if (parent.isValid() && parent.column() != 0)
        return QModelIndex();

    if( row < 0) row = rowCount(parent);

    gnaTreeItem *parentItem = getItem(parent);
    if(parentItem)
        childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}


QModelIndex DataModel::parent(const QModelIndex &child) const
{
    if (!child.isValid())
        return QModelIndex();

    gnaTreeItem *childItem = getItem(child);
    gnaTreeItem *parentItem = childItem->parent();

    if (!parentItem || parentItem == m_rootItem)
        return QModelIndex();

    return createIndex(parentItem->childNumber(), 0, parentItem);
}

bool DataModel::insertRows(int position, int rows, const QModelIndex &parent)
{
    if( position < 0 ) position = rowCount(parent);

    gnaTreeItem *parentItem = getItem(parent);
    bool success = false;

    if( parentItem)
    {
        beginInsertRows(parent, position, position + rows);
        success = parentItem->insertChildren(position, rows, parentItem->columnCount());
        endInsertRows();
    }

    reset();

    return success;
}

bool DataModel::removeRows(int row, int count, const QModelIndex &parent)
{
    gnaTreeItem *parentItem = getItem(parent);
    bool success = false;

    if( parentItem)
    {
        beginRemoveRows(parent, row, row + count);
        success = parentItem->removeChildren(row, count);
        endRemoveRows();
    }

    reset();

    return success;
}


int DataModel::rowCount(const QModelIndex &parent) const
{
    gnaTreeItem *parentItem = getItem(parent);

    if(parentItem)
        return parentItem->childCount();
    else if( m_rootItem)
        return m_rootItem->childCount();

    return 0;
}

int DataModel::columnCount(const QModelIndex &parent) const
{
    gnaTreeItem * pItem = getItem(parent);

    if( pItem )
        return pItem->columnCount();

    if(m_rootItem)
        return m_rootItem->columnCount();

    return 0;
}

QVariant DataModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole && role != Qt::EditRole)
        return QVariant();

    gnaTreeItem *item = getItem(index);
    if( item == NULL )
        return QVariant();

    gnaVariant gnaVar;
    gnaVar= item->data(index.column());
    return QVariant(gnaVar.toString());
}

QVariant DataModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        if(m_rootItem)
        {
            QString qString;

            gnaVariant gnaVar = m_rootItem->data(section);
            qString = gnaVar.toString();
            return QVariant(qString);
        }
    }

    return QVariant();
}

Qt::ItemFlags DataModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

gnaTreeItem *DataModel::getItem(const QModelIndex &index) const
{
    if (index.isValid()) {
        gnaTreeItem *item = static_cast<gnaTreeItem*>(index.internalPointer());
        if (item) return item;
    }

    return m_rootItem;
}

bool DataModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    bool result = false;

    gnaTreeItem * item = getItem(index);

    if (role != Qt::EditRole || !item)
        return result;

    string aString = value.toString().toStdString();
    gnaVariant gnaVar = aString.c_str();

    result = item->setData(index.column(), gnaVar);

    if (result)
        emit dataChanged(index, index);

    return result;
}

bool DataModel::setHeaderData(int section, Qt::Orientation orientation,
                              const QVariant &value, int role)
{
    bool result = false;

    if (role != Qt::EditRole || orientation != Qt::Horizontal || !m_rootItem )
        return result;

    gnaVariant aVar = value.toString().toStdString();
    result = m_rootItem->setData(section, aVar );

    if (result)
        emit headerDataChanged(orientation, section, section);

    return result;
}

