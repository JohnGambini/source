#ifndef TRACKSETTINGSWIDGET_H
#define TRACKSETTINGSWIDGET_H

#include "gnatreeitem.h"

#include <QTableWidget>

class TrackSettingsWidget : public QTableWidget
{
    Q_OBJECT
public:
    explicit TrackSettingsWidget(QWidget *parent = 0);
    int addTrack(int);
    void setRoot(gnaTreeItem * root);
    int getPortID(int index );

signals:
    void dataChanged();
    void selectionChange(QModelIndexList &);
    void noteLabelsChanged(bool,int);
    void rateChange(int,int); //track,rate

public slots:
        void trackSelectionChanged();
        void setSelectedIndexes(QModelIndexList &);

private:
    gnaTreeItem * m_root;
    QStringList m_midiDeviceNames;
    QList<int> m_midiPortsToUse;

};

#endif // TRACKSETTINGSWIDGET_H
