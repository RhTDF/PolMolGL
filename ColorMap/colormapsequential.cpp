#include "colormapsequential.h"

ColorMapSequential::ColorMapSequential()
{
    leftCP.position = 0.1f;
    rightCP.position = 1.0f;
    leftCP.color = Qt::white;
}

QColor ColorMapSequential::getColor(float value)
{
    float lightness = normalize(0.0f, 1.0f, rightCP.position, leftCP.position, value);
    if (lightness > 1.0) lightness = 1.0;
    if (lightness < 0.0) lightness = 0.0;
    QColor col;
    col.setHslF(rightCP.color.hueF(), 1.0, lightness);
    return col;
}

QColor ColorMapSequential::getColor(double value)
{
    float lightness = normalize(0.0f, 1.0f, rightCP.position, leftCP.position, float(value));
    if (lightness > 1.0) lightness = 1.0;
    if (lightness < 0.0) lightness = 0.0;
    QColor col;
    col.setHslF(rightCP.color.hueF(), 1.0, lightness);
    return col;
}

QColor ColorMapSequential::getColor(int value)
{

}

int ColorMapSequential::getType()
{
    return ColorMap::TYPE_SEQUENTIAL;
}

int ColorMapSequential::getNumberOfColors()
{
    return 1;
}

void ColorMapSequential::setLeftPosition(float pos)
{
    leftCP.position = pos;
}

void ColorMapSequential::setRightPosition(float pos)
{
    rightCP.position = pos;
}

void ColorMapSequential::setRightColor(QColor color)
{
    rightCP.color = color;
}
