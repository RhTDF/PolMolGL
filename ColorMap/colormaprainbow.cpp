#include "colormaprainbow.h"

ColorMapRainbow::ColorMapRainbow()
{
    leftCP.color.setHsvF(0, 0.5, 0.5);
    rightCP.color.setHsv(1.0, 0.5, 0.5);
    leftCP.position = 0.0;
    rightCP.position = 1.0;
    m_Saturation = 0.5;
    m_Value = 0.5;
}

QColor ColorMapRainbow::getColor(float value)
{
    float hue = normalize(0.0f, 1.0f,leftCP.position, rightCP.position, value);
    if (hue > 1.0) hue = 1.0f;
    if (hue < 0.0) hue = 0.0f;
    QColor col;
    col.setHsvF(hue, m_Saturation, m_Value);
    return col;
}

QColor ColorMapRainbow::getColor(double value)
{
    float hue = normalize(0.0f, 1.0f,leftCP.position, rightCP.position, float(value));
    if (hue > 1.0) hue = 1.0f;
    if (hue < 0.0) hue = 0.0f;
    QColor col;
    col.setHsvF(hue, m_Saturation, m_Value);
    return col;
}

QColor ColorMapRainbow::getColor(int value)
{

}

int ColorMapRainbow::getType()
{
    return TYPE_RAINBOW;
}

int ColorMapRainbow::getNumberOfColors()
{
    return -1;
}

void ColorMapRainbow::setLeftPosition(float pos)
{
    leftCP.position = pos;
}

void ColorMapRainbow::setRightPosition(float pos)
{
    rightCP.position = pos;
}

float ColorMapRainbow::getLeftPosition()
{
    return leftCP.position;
}

float ColorMapRainbow::getRightPosition()
{
    return rightCP.position;
}

void ColorMapRainbow::setSaturation(float sat)
{
    m_Saturation = sat;
}

void ColorMapRainbow::setValue(float val)
{
    m_Value = val;
}
