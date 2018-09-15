#ifndef DATAMODEL_H
#define DATAMODEL_H

#include <QAbstractItemModel>

#include "gnatreeitem.h"

class DataModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit DataModel(QObject *parent = 0);
    explicit DataModel(gnaTreeItem * root, QObject *parent = 0);
    virtual QModelIndex index(int row, int column, const QModelIndex &parent) const;
    virtual QModelIndex parent(const QModelIndex &child) const;
    virtual bool insertRows(int position, int rows, const QModelIndex &parent);
    virtual bool removeRows(int row, int count, const QModelIndex &parent);
    virtual int rowCount(const QModelIndex &parent) const;
    virtual int columnCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;

    gnaTreeItem * getItem(const QModelIndex &index) const;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role);
    virtual bool setHeaderData(int section, Qt::Orientation orientation,const QVariant &value, int role = Qt::EditRole);

signals:
    
public slots:

private:
    gnaTreeItem * m_rootItem;

};

#endif // DATAMODEL_H
