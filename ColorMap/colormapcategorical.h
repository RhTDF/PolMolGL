#ifndef COLORMAPCATEGORICAL_H
#define COLORMAPCATEGORICAL_H

#include "colormap.h"


class ColorMapCategorical : public ColorMap
{
public:
    ColorMapCategorical();
    QColor getColor(float value) override;
    QColor getColor(double value) override;
    QColor getColor(int value) override;
    int getType() override;
    int getNumberOfColors() override;
    void addControlPoint(QColor color, float position);
    void removeControlPoint(int index);
    void moveControlPoint(int index, float newPos);
    void changeControlPointColor(int index, QColor color);

private:
    QVector<ControlPoint> m_ControlPoints;

};

#endif // COLORMAPCATEGORICAL_H
