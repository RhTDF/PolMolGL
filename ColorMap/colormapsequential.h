#ifndef COLORMAPSEQUENTIAL_H
#define COLORMAPSEQUENTIAL_H

#include "ColorMap/colormap.h"

class ColorMapSequential :public ColorMap
{
public:
    ColorMapSequential();
    QColor getColor(float value) override;
    QColor getColor(double value) override;
    QColor getColor(int value) override;
    int getType() override;
    int getNumberOfColors() override;
    void setLeftPosition(float pos);
    void setRightPosition(float pos);
    void setRightColor(QColor color);

private:
    ControlPoint leftCP;
    ControlPoint rightCP;
};

#endif // COLORMAPSEQUENTIAL_H
