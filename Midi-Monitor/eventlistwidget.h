#ifndef EVENTLISTWIDGET_H
#define EVENTLISTWIDGET_H

#include "pm_common/portmidi.h"

#include <QTableView>

class EventListWidget : public QTableView
{
    Q_OBJECT
public:
    explicit EventListWidget(QWidget *parent = 0);
    void addRow(PmEvent);


signals:
    
public slots:
    void clear();

    
};

#endif // EVENTLISTWIDGET_H
