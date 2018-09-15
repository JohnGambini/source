#ifndef PLAYCLASS_H
#define PLAYCLASS_H

#include <QObject>

#include "gnatreeitem.h"

class PlayClass : public QObject
{
    Q_OBJECT
public:
    explicit PlayClass(gnaTreeItem * root, bool * pbStop, unsigned long offsetTime, QObject *parent = 0);
    
signals:
    void clockTick(unsigned long);
    void processStopped();
    void error(QString);
    
public slots:
    void play();
    
private:
    gnaTreeItem * m_root;
    bool * m_pbStop;
    unsigned long m_offsetTime;

};

#endif // PLAYCLASS_H
