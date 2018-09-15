#ifndef PATCHCHNLWIDGET_H
#define PATCHCHNLWIDGET_H

#include <QSpinBox>
#include <QComboBox>
#include <QPushButton>
#include <QSlider>

#include "gnatreeitem.h"

class PatchChnlWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PatchChnlWidget(QWidget *parent = 0);
    void setPortList( QStringList & deviceNames, QList<int> & portsToUse );
    void setRoot( gnaTreeItem * item );
    void resizeEvent(QResizeEvent * event);
    void addPatchList(QComboBox * comboBox);
    void setChannel(int value);

signals:
    void dataChanged();
    void channelChange(int,int);
    void noteLabelsChanged( bool, int );
    void rateChange(int, int); //track, rate
    
public slots:
    void patchValueChanged(int value);
    void channelValueChanged(int value);
    void portValueChanged(int value);
    void rateValueChanged(bool value);
    void muteValueChanged(bool value);
    void volumeChanged(int value);
    void panChanged(int value);
    void noteLabelsClicked(bool value);

private:
    QComboBox * m_patchSpinBox;
    QSpinBox * m_chnlSpinBox;
    QPushButton * m_rateButton;
    QPushButton * m_muteButton;
    QComboBox * m_portComboBox;
    QSlider * m_volumneSlider;
    QSlider * m_panSlider;
    QPushButton * m_noteLabels;
    gnaTreeItem * m_root;
    int m_noteOffset;
    int m_rate;
    
};

#endif // PATCHCHNLWIDGET_H
