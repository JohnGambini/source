#ifndef PROJECTHEADER_H
#define PROJECTHEADER_H

#include "header.h"

#include <QPushButton>

class ProjectHeader : public Header
{
    Q_OBJECT
public:
    explicit ProjectHeader(QWidget * parent = 0);
    void addEnabled(bool);
    void delEnabled(bool);

signals:
    void headerButtonPressed(int id);

public slots:

private:
    QPushButton * m_newButton;
    QPushButton * m_openButton;
    QPushButton * m_saveButton;
    QPushButton * m_addButton;
    QPushButton * m_delButton;
};

#endif // PROJECTHEADER_H
