#include "colormapcategorical.h"

ColorMapCategorical::ColorMapCategorical()
{

}

QColor ColorMapCategorical::getColor(float value)
{
    float minPos = 0.0f;
    for (auto& cp : m_ControlPoints) {
        if (cp.position <= value) {
            if (cp.position > minPos) minPos = cp.position;
        }
    }
    for (auto& cp : m_ControlPoints) {
        if (cp.position <= value && cp.position >= minPos) {
            return cp.color;
        }
    }
}

QColor ColorMapCategorical::getColor(double value)
{
    float minPos = 0.0f;
    for (auto& cp : m_ControlPoints) {
        if (cp.position <= value) {
            if (cp.position > minPos) minPos = cp.position;
        }
    }
    for (auto& cp : m_ControlPoints) {
        if (cp.position <= value && cp.position >= minPos) {
            return cp.color;
        }
    }
}

QColor ColorMapCategorical::getColor(int value)
{
    return m_ControlPoints[value].color;
}

int ColorMapCategorical::getType()
{
    return ColorMap::TYPE_QUALITIVE;
}

int ColorMapCategorical::getNumberOfColors()
{
    return m_ControlPoints.size();
}

void ColorMapCategorical::addControlPoint(QColor color, float position)
{
    ControlPoint cp;
    cp.color = color;
    cp.position = position;
    m_ControlPoints.append(cp);
}

void ColorMapCategorical::removeControlPoint(int index)
{
    if (index < m_ControlPoints.size() && index >= 0) {
        m_ControlPoints.remove(index);
    }
}

void ColorMapCategorical::moveControlPoint(int index, float newPos)
{
    if (index < m_ControlPoints.size() && index >= 0) {
        m_ControlPoints[index].position = newPos;
    }
}

void ColorMapCategorical::changeControlPointColor(int index, QColor color)
{
    if (index < m_ControlPoints.size() && index >= 0) {
        m_ControlPoints[index].color = color;
    }
}
