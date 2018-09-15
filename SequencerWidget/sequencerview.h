#ifndef SEQUENCERVIEW_H
#define SEQUENCERVIEW_H

#include <QWidget>

#include "sequencerheader.h"
#include "tracksettingswidget.h"
#include "sequencerwidget.h"
#include "playclass.h"
#include "gnatreeitem.h"

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
    void bpmChange(int);
    void errorString(QString errMsg);
    void setRoot(gnaTreeItem *);
    void pause();
    void buttonPressed(int);


private:
    gnaTreeItem * m_root;
    bool m_bStop;
    SequencerHeader m_header;

    TrackSettingsWidget * m_trackSettings;
    SequencerWidget * m_sequencerWidget;
    
};

#endif // SEQUENCERVIEW_H
