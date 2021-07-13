#include "colormapdivergent.h"

ColorMapDivergent::ColorMapDivergent()
{

}

ColorMapDivergent::ColorMapDivergent(QColor leftColor, float leftPos, QColor rightColor, float rightPos, float midPos)
{
    setColors(leftColor, rightColor);
    setPositions(leftPos, rightPos, midPos);
}

void ColorMapDivergent::setColors(QColor leftColor, QColor rightColor)
{
    setLeftColor(leftColor);
    setRightColor(rightColor);
    middleCP.color = Qt::white;
}

void ColorMapDivergent::setLeftColor(QColor color)
{
    leftCP.color = color;
}

void ColorMapDivergent::setRightColor(QColor color)
{
    rightCP.color = color;
}

void ColorMapDivergent::setPositions(float leftPos, float rightPos, float midPos)
{
    setLeftPosition(leftPos);
    setRightPosition(rightPos);
    setMidPosition(midPos);
}

void ColorMapDivergent::setLeftPosition(float pos)
{
    leftCP.position = pos;
}

void ColorMapDivergent::setRightPosition(float pos)
{
    rightCP.position = pos;
}

void ColorMapDivergent::setMidPosition(float pos)
{
    middleCP.position = pos;
}

QColor ColorMapDivergent::getLeftColor()
{
    return leftCP.color;
}

QColor ColorMapDivergent::getRightColor()
{
    return rightCP.color;
}

float ColorMapDivergent::getLeftPosition()
{
    return leftCP.position;
}

float ColorMapDivergent::getRightPosition()
{
    return rightCP.position;
}

float ColorMapDivergent::getMidPosition()
{
    return middleCP.position;
}

QColor ColorMapDivergent::getColor(float value)
{
    float s;
    if (value < leftCP.position) {
        s = normalize(0.0f, leftCP.position, 0.0f, 0.1f, value);
    } else if (value < middleCP.position) {
        s = normalize(leftCP.position, middleCP.position, 0.1f, 0.5f, value);
    } else if (value < rightCP.position) {
        s = normalize(middleCP.position, rightCP.position, 0.5f, 0.9f, value);
    } else if (value <= 1) {
        s = normalize(rightCP.position, 1.0f, 0.9f, 1.0f, value);
    }
    return interpolate(leftCP.color, rightCP.color, s);
}

QColor ColorMapDivergent::getColor(double value)
{
    float s;
    if (value < leftCP.position) {
        s = normalize(0.0, double(leftCP.position), 0.0, 0.1, value);
    } else if (value < middleCP.position) {
        s = normalize(double(leftCP.position), double(middleCP.position), 0.1, 0.5, value);
    } else if (value < rightCP.position) {
        s = normalize(double(middleCP.position), double(rightCP.position), 0.5, 0.9, value);
    } else if (value <= 1) {
        s = normalize(double(rightCP.position), 1.0, 0.9, 1.0, value);
    }
    return interpolate(leftCP.color, rightCP.color, s);
}

QColor ColorMapDivergent::getColor(int value)
{
    float s;
    if (value < leftCP.position) {
        s = normalize(0.0f, leftCP.position, 0.0f, 0.1f, float(value));
    } else if (value < middleCP.position) {
        s = normalize(leftCP.position, middleCP.position, 0.1f, 0.5f, float(value));
    } else if (value < rightCP.position) {
        s = normalize(middleCP.position, rightCP.position, 0.5f, 0.9f, float(value));
    } else if (value <= 1) {
        s = normalize(rightCP.position, 1.0f, 0.9f, 1.0f, float(value));
    }
    return interpolate(leftCP.color, rightCP.color, s);
}

int ColorMapDivergent::getType()
{
    return TYPE_DIVERGENT;
}

int ColorMapDivergent::getNumberOfColors()
{
    return 2;
}

QColor ColorMapDivergent::interpolate(QColor c1, QColor c2, float s)
{
    QVector3D lab1 = toCIELABfromRGB(QVector3D(c1.redF(), c1.greenF(), c1.blueF()));
    QVector3D lab2 = toCIELABfromRGB(QVector3D(c2.redF(), c2.greenF(), c2.blueF()));

    QVector3D msh1 = toMSHfromCIELAB(lab1.x(), lab1.y(), lab1.z());
    QVector3D msh2 = toMSHfromCIELAB(lab2.x(), lab2.y(), lab2.z());

    if (msh1.y() > 0.05 && msh2.y() > 0.05 && radDiff(msh1.z(),msh2.z()) > M_PI/3.0) {
        float midM = qMax(msh1.x(), msh2.x());
        //qDebug() << "mid:" << midM;
        midM = qMax(88.0f, midM);
        if (s < 0.5) {
            msh2 = QVector3D(midM, 0.0, 0.0);
            s = 2.0*s;
        } else {
            msh1 = QVector3D(midM, 0.0, 0.0);
            s = 2.0 * s - 1.0;
        }
    }

    if (msh1.y() < 0.05 && msh2.y() > 0.05) {
        msh1.setZ(adjustMSHue(msh2.x(),msh2.y(),msh2.z(),msh1.x()));
    } else if (msh2.y() < 0.05 && msh1.y() > 0.05) {
        msh2.setZ(adjustMSHue(msh1.x(),msh1.y(),msh1.z(),msh2.x()));
    }

    float tempM = (1-s)*msh1.x() + s*msh2.x();
    float tempS = (1-s)*msh1.y() + s*msh2.y();
    float tempH = (1-s)*msh1.z() + s*msh2.z();

    QVector3D tempLAB = toCIELABfromMSH(tempM, tempS, tempH);
    QVector3D resultRGB = toRGBfromCIELAB(tempLAB);

    QColor result;
    //qDebug() << resultRGB.x() << resultRGB.y() << resultRGB.z();
    result.setRgbF(resultRGB.x(),resultRGB.y(),resultRGB.z());
    return  result;

}

QVector3D ColorMapDivergent::toXYZfromRGB(float r, float g, float b)
{
    if (r > 0.04045) r = qPow((r + 0.055)/1.055,2.4);
    else r = r/12.92;
    if (g > 0.04045) g = qPow((g + 0.055)/1.055,2.4);
    else g = g/12.92;
    if (b > 0.04045) b = qPow((b + 0.055)/1.055,2.4);
    else b = b/12.92;

    float x = r * 0.4124f + g * 0.3576f + b * 0.1805f;
    float y = r * 0.2126f + g * 0.7152f + b * 0.0722f;
    float z = r * 0.0193f + g * 0.1192f + b * 0.9505f;
    return QVector3D(x,y,z);
}

QVector3D ColorMapDivergent::toRGBfromXYZ(float x, float y, float z)
{
    float r = x *  3.2406f + y * -1.5372f + z * -0.4986f;
    float g = x * -0.9689f + y *  1.8758f + z *  0.0415f;
    float b = x *  0.0557f + y * -0.2040f + z *  1.0570f;

    if (r > 0.0031308) r = 1.055 * qPow(r, 1.0/2.4) - 0.055;
    else r = 12.92 * r;
    if (g > 0.0031308) g = 1.055 * qPow(g, 1.0/2.4) - 0.055;
    else g = 12.92 * g;
    if (b > 0.0031308) b = 1.055 * qPow(b, 1.0/2.4) - 0.055;
    else b = 12.92 * b;

    float maxVal = r;
    if (maxVal < g) maxVal = g;
    if (maxVal < b) maxVal = b;
    if (maxVal > 1.0) {
      r = r/maxVal;
      g = g/maxVal;
      b = b/maxVal;
    }
    if (r<0) r=0;
    if (g<0) g=0;
    if (b<0) b=0;

    return QVector3D(r, g, b);
}

QVector3D ColorMapDivergent::toCIELABfromXYZ(float x, float y, float z)
{
    float xn = 0.9505f;
    float yn = 1.000f;
    float zn = 1.089f;

    float fx = toCIELABfromXYZAux(x/xn);
    float fy = toCIELABfromXYZAux(y/yn);
    float fz = toCIELABfromXYZAux(z/zn);

    float l = 116 * (fy - 16.0/116.0);
    float a = 500 * (fx - fy);
    float b = 200 * (fy - fz);

    return QVector3D(l, a, b);
}

float ColorMapDivergent::toCIELABfromXYZAux(float x)
{
    if (x > 0.008856) return qPow(x,1.0/3.0);
    else return 7.787 * x + 16.0/116/0;
}

QVector3D ColorMapDivergent::toXYZfromCIELAB(float l, float a, float b)
{
    float y = (l+16)/116.0;
    float x = a / 500 + y;
    float z = y - b / 200;

    float x3 = qPow(x, 3);
    float y3 = qPow(y, 3);
    float z3 = qPow(z, 3);

    float xn = 0.9505f;
    float yn = 1.000f;
    float zn = 1.089f;

    if (y3 > 0.008856) y = y3;
    else y = (y - 16.0/116.0)/7.787;

    if (x3 > 0.008856) x = x3;
    else x = (x - 16.0/116.0)/7.787;

    if (z3 > 0.008856) z = z3;
    else z = (z - 16.0/116.0)/7.787;

    x = xn * x;
    y = yn * y;
    z = zn * z;

    return QVector3D(x, y, z);
}

QVector3D ColorMapDivergent::toMSHfromCIELAB(float l, float a, float b)
{
    float m = qSqrt(qPow(l,2) + qPow(a,2) + qPow(b,2));
    float s = qAcos(l/m);
    float h = qAtan2(b, a);
    return QVector3D(m,s,h);
}

QVector3D ColorMapDivergent::toCIELABfromMSH(float m, float s, float h)
{
    float l = m * qCos(s);
    float a = m * qSin(s) * qCos(h);
    float b = m * qSin(s) * qSin(h);
    return QVector3D(l,a,b);
}

QVector3D ColorMapDivergent::toRGBfromCIELAB(QVector3D LABVec)
{
    QVector3D xyz = toXYZfromCIELAB(LABVec.x(), LABVec.y(), LABVec.z());
    return toRGBfromXYZ(xyz.x(), xyz.y(), xyz.z());
}

QVector3D ColorMapDivergent::toCIELABfromRGB(QVector3D RGBVec)
{
    QVector3D xyz = toXYZfromRGB(RGBVec.x(), RGBVec.y(), RGBVec.z());
    return toCIELABfromXYZ(xyz.x(), xyz.y(), xyz.z());
}

float ColorMapDivergent::radDiff(float h1, float h2)
{
    QVector2D v1(qCos(h1), qSin(h1));
    QVector2D v2(qCos(h2), qSin(h2));
    return qAcos(QVector2D::dotProduct(v1, v2));
}

float ColorMapDivergent::adjustMSHue(float satM, float satS, float satH, float unsatM)
{
    if (satM >= unsatM) {
        //qDebug() << "true";
        return satH;
    } else {
        //qDebug() << "false";
        float hueSpin = (satS * qSqrt(qPow(unsatM,2)-qPow(satM,2)))/(satM * qSin(satS));
        if (satH > -M_PI/3.0) return satH + hueSpin;
        else return satH - hueSpin;
    }
}
