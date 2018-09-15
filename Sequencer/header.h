#ifndef HEADER_H
#define HEADER_H

#include <QWidget>
#include <QBoxLayout>

class Header : public QWidget
{
    Q_OBJECT
public:
    explicit Header(QWidget *parent = 0);
    void paintEvent(QPaintEvent *);
    void addWidget(QWidget * widget);
    void setAlignment(QWidget * widget, Qt::Alignment flg){ m_layout->setAlignment(widget,flg); }

signals:
    void buttonPressed(int id);

public slots:
    
private:
    QBoxLayout * m_layout;

};

#endif // HEADER_H
