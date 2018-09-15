#ifndef PROJECTWIDGET_H
#define PROJECTWIDGET_H

#include <QTableView>
#include <QLineEdit>
#include <QMenu>

#include "gnatreeitem.h"

class ProjectWidget : public QTableView
{
    Q_OBJECT
public:
    explicit ProjectWidget(QWidget *parent = 0);
    bool insertSequence();
    bool addSequence();
    bool delSelected();
    void setRoot(gnaTreeItem * root);
    virtual void resizeEvent ( QResizeEvent * event );
    virtual void contextMenuEvent(QContextMenuEvent *event);

signals:
    void selectedItemChanged(gnaTreeItem * item);
    void selectedDataChanged(const QModelIndex &, const QModelIndex &);
    void itemDeleted(gnaTreeItem *);
    void dataChanged();

public slots:
    void selectionChanged ( const QItemSelection & selected, const QItemSelection & deselected );
    void copySequence();
    //void editCell(int,int);
    //void editingFinished();

private:
    QMenu m_menu;
    QLineEdit * m_editWidget;

};

#endif // PROJECTWIDGET_H
