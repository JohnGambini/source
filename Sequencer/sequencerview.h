#ifndef SEQUENCERVIEW_H
#define SEQUENCERVIEW_H

#include "gnatreeitem.h"

#include <QWidget>
#include <QStringList>

#include "sequencerheader.h"
#include "sequencerwidget.h"
#include "tracksettingswidget.h"

class SequencerView : public QWidget
{
    Q_OBJECT
public:
    explicit SequencerView(QWidget *parent = 0);
    void addTrack(int);

signals:
    void dataChanged();
    void setThreadRoot(gnaTreeItem *);
    void stopButtonPressed();

public slots:
    void stopPlaying();
    void bpmChange(int);
    void stepsChange(int);
    void channelChange(int,int);
    void setRoot(gnaTreeItem *);
    void buttonPressed(int);
    void setQueueLabel(QString val);
    void itemDeleted(gnaTreeItem *);
    void allNotesOff();

private:
    bool m_isPlaying;
    gnaTreeItem * m_root;
    SequencerHeader * m_header;
    TrackSettingsWidget * m_trackSettings;
    SequencerWidget * m_sequencerWidget;
};

#endif // SEQUENCERVIEW_H
