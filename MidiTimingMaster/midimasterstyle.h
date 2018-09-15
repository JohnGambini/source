#ifndef MIDIMASTERSTYLE_H
#define MIDIMASTERSTYLE_H

#include <QPlastiqueStyle>

class MidiMasterStyle : public QPlastiqueStyle
{
    Q_OBJECT

public:
    MidiMasterStyle();
    void polish(QPalette &palette);
    void polish(QWidget *widget);
    void unpolish(QWidget *widget);
    int pixelMetric(PixelMetric metric, const QStyleOption *option, const QWidget *widget) const;
    int styleHint(StyleHint hint, const QStyleOption *option,
                  const QWidget *widget, QStyleHintReturn *returnData) const;

    void drawPrimitive(PrimitiveElement element, const QStyleOption *option,
                       QPainter *painter, const QWidget *widget) const;

    void drawControl(ControlElement element, const QStyleOption *option,
        QPainter *painter, const QWidget *widget) const;

};

#endif // MIDIMASTERSTYLE_H

