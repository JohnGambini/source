#ifndef NOTEWIDGET_H
#define NOTEWIDGET_H

#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QDial>

#include "gnatreeitem.h"

class NoteWidget : public QWidget
{
    Q_OBJECT
public:
    explicit NoteWidget(QWidget *parent = 0);
    void resizeEvent(QResizeEvent * event);
    void setRoot(gnaTreeItem * item );
    void noteOn(bool value);
    void noteOff(bool value);
    void setLabel(QString label);
    int note();
    void setDrums();
    void setNotes();

signals:
    void dataChanged();
    
public slots:
    void valueChanged(int value);
    void pushChecked(bool);


private:
    QPushButton * m_pushButton;
    QComboBox * m_noteSpinBox;
    QDial * m_dial;
    gnaTreeItem * m_root;
    QStringList m_notes;
    QStringList m_drums;


};

#endif // NOTEWIDGET_H
