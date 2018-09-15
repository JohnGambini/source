#ifndef PLAYCLASS_H
#define PLAYCLASS_H

#include <QObject>
#include <QTime>
#include <QTimer>

#include "gnatreeitem.h"

#include "libmidi_global.h"
#include "driverwin32.h"
#include "msg.h"
#include "deque"

using namespace jdkmidi;

struct structNotesOff
{
    int pos;
    int patch;
    int chnl;
    int port;
    int note;
};


class PlayClass : public QObject
{
    Q_OBJECT
public:
    explicit PlayClass(gnaTreeItem * root, QObject * parent = 0);
    virtual ~PlayClass();
    void addPort(int portId);
    void setBPM(int bpm);

signals:
    void finished();
    void error(QString err);
    void noteOn(int,int,int,int);
    void noteOff(int,int,int,int);
    void allNotesOff();
    void patchChange(int,int,int);
    void setQueueLabel(QString val);
    void stopped();

public slots:
    void play();
    void stopPlaying();
    void timerEvent();
    void setRoot( gnaTreeItem * newroot);

private:
    vector<int> m_trackPosition;
    vector<deque<structNotesOff *> *> m_notesOff;
    vector<int> m_rateCount;
    vector<int> m_queuedRateCount;
    vector<int> m_patches;
    vector<int> m_portIds;
    vector<MIDIDriverWin32 *> m_midiDrivers;
    bool m_noteOnOff;
    int m_BPM;
    int m_noteOffset;
    bool m_stop;
    gnaTreeItem * m_root;
    gnaTreeItem * m_nextRoot;
    int m_realPositionCount;

};


#endif // PLAYCLASS_H
