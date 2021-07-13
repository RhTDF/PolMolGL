#include "colormap.h"
#include <qdebug.h>

ColorMap::ColorMap()
{

}

ColorMap::ColorMap(int type, double vmin, double vmax)
{
    m_Type = type;
    m_ValueMin = vmin;
    m_ValueMax = vmax;
}

ColorMap::ColorMap(ColorMap *map, double vmin, double vmax)
{
    m_Type = map->m_Type;
    m_ValueMin = vmin;
    m_ValueMax = vmax;
    m_ColorList = map->m_ColorList;
}

ColorMap::~ColorMap()
{

}

void ColorMap::addColor(QColor color)
{
    m_ColorList.append(color);
}

void ColorMap::addColor(QVector3D color)
{
    m_ColorList.append(QColor(color.x(),color.y(),color.z()));
}



QVector3D ColorMap::getColorVec(int value)
{

}

QVector3D ColorMap::getColorVec(double value)
{

}

QVector3D ColorMap::getColorVec(float value)
{

}



int ColorMap::normalize(int rmin, int rmax, int tmin, int tmax, int val)
{
    return tmin + qRound(double(double(tmax-tmin)*double(val-rmin)/double(rmax-rmin)));
}

double ColorMap::normalize(double rmin, double rmax, double tmin, double tmax, double val)
{
    return tmin + (tmax-tmin)*(val-rmin)/(rmax-rmin);
}

float ColorMap::normalize(float rmin, float rmax, float tmin, float tmax, float val)
{
    return tmin + (tmax-tmin)*(val-rmin)/(rmax-rmin);
}
