#include "midimasterstyle.h"

#include <QStyleOption>
#include <QPainter>
#include <QPushButton>
#include <QComboBox>

MidiMasterStyle::MidiMasterStyle()
{
}

void MidiMasterStyle::polish(QPalette & palette )
{
    QPlastiqueStyle::polish(palette);
}

void MidiMasterStyle::polish(QWidget * widget )
{
    if(qobject_cast<QPushButton *>(widget))
        widget->setAttribute(Qt::WA_Hover, true);
}

void MidiMasterStyle::unpolish(QWidget * widget )
{
    if (qobject_cast<QPushButton *>(widget))
        widget->setAttribute(Qt::WA_Hover, false);
}

int MidiMasterStyle::pixelMetric(PixelMetric metric, const QStyleOption *option, const QWidget *widget) const
{
    switch (metric)
    {
    case PM_ComboBoxFrameWidth:
        return QPlastiqueStyle::pixelMetric(metric, option, widget) + 4;
    case PM_ScrollBarExtent:
        return QPlastiqueStyle::pixelMetric(metric, option, widget) -2;
    default:
        return QPlastiqueStyle::pixelMetric(metric, option, widget);
    }
}

int MidiMasterStyle::styleHint(StyleHint hint, const QStyleOption *option, const QWidget *widget, QStyleHintReturn *returnData) const
{
    switch (hint)
    {
    case SH_DitherDisabledText:
        return int(false);
    case SH_EtchDisabledText:
        return int(true);
    default:
        return QPlastiqueStyle::styleHint(hint, option, widget, returnData);
    }
}

void MidiMasterStyle::drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
    //int delta = (option->state & State_MouseOver) ? 64 : 0;
    QColor highlight(255, 255, 255, 8);
    QColor black(0,0,0,255);
    QBrush brush;
    QFont aFont;
    QLinearGradient linearGradient(0,0,0,1.75);

    //const QStyleOptionButton *buttonOption = qstyleoption_cast<const QStyleOptionButton *>(option);

    switch (element)
    {
    case PE_Frame:
        QPlastiqueStyle::drawPrimitive(element, option, painter, widget);
        break;

    case PE_PanelItemViewItem:
        /*
        if(option->state & State_Selected)
        {
            linearGradient.setColorAt(0.0, QColor(128,128,128));
            linearGradient.setColorAt(0.6, QColor(96,96,96));
        }
        else
        {
            linearGradient.setColorAt(0.0, QColor(240,240,240));
            linearGradient.setColorAt(0.6, QColor(192,192,192));
        }
        //if(qobject_cast<const PatchChnlWidget *>(widget))
        //{
            linearGradient.setCoordinateMode(QGradient::ObjectBoundingMode);
            brush = linearGradient;
            //brush = black;
            painter->setBrush(brush);
            painter->drawRect(option->rect);
        //}
        */
        QPlastiqueStyle::drawPrimitive(element, option, painter, widget);
        break;

    case PE_PanelButtonCommand:
        if (option->state & State_MouseOver)
        {
            brush = highlight;
            painter->setBrush(brush);
            QRect rect = option->rect;
            rect.setRight(rect.right()-1);
            rect.setBottom(rect.bottom()-1);
            painter->drawRoundedRect(rect,4.0,4.0);
        }
        else
            QPlastiqueStyle::drawPrimitive(element, option, painter, widget);
        break;

    default:
        QPlastiqueStyle::drawPrimitive(element, option, painter, widget);

    }
}

void MidiMasterStyle::drawControl(ControlElement element, const QStyleOption *option,
    QPainter *painter, const QWidget *widget) const
{
    switch (element)
    {
    case CE_PushButtonLabel:
        {
            /*
            QStyleOptionButton myButtonOption;
            const QStyleOptionButton *buttonOption =
                    qstyleoption_cast<const QStyleOptionButton *>(option);
            if (buttonOption)
            {
                myButtonOption = *buttonOption;
                if (myButtonOption.palette.currentColorGroup()
                        != QPalette::Disabled) {
                    if (myButtonOption.state & (State_Sunken | State_On))
                    {
                        myButtonOption.palette.setBrush(QPalette::ButtonText,
                                myButtonOption.palette.brightText());
                    }
                }
            }
            QPlastiqueStyle::drawControl(element, &myButtonOption, painter, widget);
            */
           QPlastiqueStyle::drawControl(element, option, painter, widget);
        }
        break;

    default:
        QPlastiqueStyle::drawControl(element, option, painter, widget);
    }
}
