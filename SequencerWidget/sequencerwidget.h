#ifndef SEQUENCERWIDGET_H
#define SEQUENCERWIDGET_H

#include "gnatreeitem.h"

#include <QBoxLayout>
#include <QTableWidget>
#include <QThread>
#include <QLabel>

#include "sequencerheader.h"

class SequencerWidget : public QTableWidget
{
    Q_OBJECT
public:
    explicit SequencerWidget(QWidget *parent = 0);
    void addTrack(int row,int height);
    void resizeEvent(QResizeEvent * event);

signals:
    void dataChanged();
    void stopButton();
    void selectionChange(QModelIndexList &);

public slots:
    void noteOn(int x, int y, int chnl, int port);
    void noteOff(int x, int y, int chnl, int port);
    void channelChange(int track,int chnl);
    void setRoot(gnaTreeItem * root );
    void setNoteLabels(bool value,int track);
    void rateChange(int track, int rate);
    void resizeSection(int logicalIndex, int oldSize, int newSize);

    void sequencerSelectionChanged();
    void setSelectedIndexes(QModelIndexList &);


private:
    bool m_isPlaying;
    gnaTreeItem * m_root;
};

#endif // SEQUENCERWIDGET_H
