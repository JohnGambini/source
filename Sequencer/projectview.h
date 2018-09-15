#ifndef PROJECTVIEW_H
#define PROJECTVIEW_H

#include <QWidget>
#include <QModelIndex>

class ProjectHeader;
class ProjectWidget;
class QAbstractItemModel;
class gnaTreeItem;

class ProjectView : public QWidget
{
    Q_OBJECT
public:
    ProjectView(QAbstractItemModel * dataModel, QWidget * parent = 0);
    void updateActions();
    void setRoot(gnaTreeItem * root);

signals:
    void dataChanged();
    void newRoot(gnaTreeItem *);
    void itemDeleted(gnaTreeItem *);
    void newDocument();
    void openDocument();
    void saveDocument();

protected slots:
    virtual void selectedItemChanged(gnaTreeItem * item);
    virtual void selectedDataChanged(const QModelIndex & topLeft,const QModelIndex & bottomRight);
    virtual void headerButtonPressed(int id);

private:
    ProjectHeader * m_projectHeader;
    ProjectWidget * m_projectWidget;
};

#endif // PROJECTVIEW_H
