#ifndef COLORMAPRAINBOW_H
#define COLORMAPRAINBOW_H

#include "colormap.h"

class ColorMapRainbow :public ColorMap
{
public:
    ColorMapRainbow();
    QColor getColor(float value) override;
    QColor getColor(double value) override;
    QColor getColor(int value) override;
    int getType() override;
    int getNumberOfColors() override;
    void setLeftPosition(float pos);
    void setRightPosition(float pos);
    float getLeftPosition();
    float getRightPosition();
    void setSaturation(float sat);
    void setValue(float val);

private:
    ControlPoint leftCP;
    ControlPoint rightCP;
    float m_Saturation;
    float m_Value;
};

#endif // COLORMAPRAINBOW_H
