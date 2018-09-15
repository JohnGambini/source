#include "header.h"

#include <QPainter>
#include <QStyle>

Header::Header(QWidget *parent) :
    QWidget(parent), m_layout(0)
{
    setMinimumHeight(25);

    m_layout = new QBoxLayout(QBoxLayout::LeftToRight, this);
    m_layout->setMargin(0);
    m_layout->setSpacing(2);
}

void Header::addWidget(QWidget * widget)
{
    m_layout->addWidget(widget);
}

void Header::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    QLinearGradient linearGradient(0, 0, 0, 2);
    linearGradient.setColorAt(0.0, Qt::gray);
    linearGradient.setColorAt(0.6, QColor(42,42,64));
    linearGradient.setCoordinateMode(QGradient::ObjectBoundingMode);
    //QColor aColor(96,96,127);
    //QBrush aBrush(aColor);
    painter.setBrush(linearGradient);
    painter.drawRect(geometry());

    painter.end();

}



