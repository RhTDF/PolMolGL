#ifndef COLORMAPDIVERGENT_H
#define COLORMAPDIVERGENT_H

#include "colormap.h"



class ColorMapDivergent : public ColorMap
{
public:
    ColorMapDivergent();
    ColorMapDivergent(QColor leftColor, float leftPos, QColor rightColor, float rightPos, float midpos);
    void setColors(QColor leftColor, QColor rightColor);
    void setLeftColor(QColor color);
    void setRightColor(QColor color);
    void setPositions(float leftPos, float rightPos, float midPos);
    void setLeftPosition(float pos);
    void setRightPosition(float pos);
    void setMidPosition(float pos);
    QColor getLeftColor();
    QColor getRightColor();
    float getLeftPosition();
    float getRightPosition();
    float getMidPosition();
    QColor getColor(float value) override;
    QColor getColor(double value) override;
    QColor getColor(int value) override;
    int getType() override;
    int getNumberOfColors() override;
private:
    ControlPoint leftCP;
    ControlPoint middleCP;
    ControlPoint rightCP;
    QColor interpolate(QColor c1, QColor c2, float s);
    QVector3D toXYZfromRGB(float r, float g, float b);
    QVector3D toRGBfromXYZ(float x, float y, float z);
    QVector3D toCIELABfromXYZ(float x, float y, float z);
    float toCIELABfromXYZAux(float x);
    QVector3D toXYZfromCIELAB(float l, float a, float b);
    QVector3D toMSHfromCIELAB(float l, float a, float b);
    QVector3D toCIELABfromMSH(float m, float s, float h);
    QVector3D toRGBfromCIELAB(QVector3D LABVec);
    QVector3D toCIELABfromRGB(QVector3D RGBVec);
    float radDiff(float h1, float h2);
    float adjustMSHue(float satM, float satS, float satH, float unsatM);
};

#endif // COLORMAPDIVERGENT_H
