#ifndef MIDISETTINGS_H
#define MIDISETTINGS_H

#include "gnatreeitem.h"
#include "midi-common_global.h"

#include <QDialog>
#include <QListWidgetItem>

namespace Ui {
class MIDISettings;
}

class MIDICOMMONSHARED_EXPORT MIDISettings : public QDialog
{
    Q_OBJECT
    
public:
    explicit MIDISettings(QWidget *parent = 0);
    ~MIDISettings();

    enum StyleBits
    {
        InputPorts = 0x01,
        OutputPorts = 0x02
    };

    void setStyleBits(unsigned int styleBits);
    void setRoot(gnaTreeItem * root);
    gnaFile * m_errLog;

signals:

public slots:
    void accept();
    void inSelectionChanged();
    void outSelectionChanged();
    void inItemClicked(QListWidgetItem * item);
    void outItemClicked(QListWidgetItem * item);
    void inMMCChanged(int value);
    void inMTCChanged(int value);
    void outMMCChanged(int value);
    void outMTCChanged(int value);
    void fpsChanged(int value);

private:
    Ui::MIDISettings *ui;
    gnaTreeItem * m_root;
    unsigned int m_styleBits;
    int m_moveBy;
};

#endif // MIDISETTINGS_H
