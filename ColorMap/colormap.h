#ifndef COLORMAP_H
#define COLORMAP_H
#include <QtMath>
#include <QVector>
#include <QColor>
#include <QVector3D>
#include <QVector2D>
#include <QVector>
#include <QMatrix>
#include <QMatrix3x3>
#include <QGenericMatrix>

typedef struct {
   QColor color;
   float position;
} ControlPoint;


class ColorMap
{
public:
    ColorMap();
    ColorMap(int type, double vmin, double vmax);
    ColorMap(ColorMap *map, double vmin, double vmax);
    virtual ~ColorMap();
    static int normalize(int rmin, int rmax, int tmin, int tmax, int val);
    static double normalize(double rmin, double rmax, double tmin, double tmax, double val);
    static float normalize(float rmin, float rmax, float tmin, float tmax, float val);
    void addColor(QColor color);
    void addColor(QVector3D color);
    virtual QColor getColor(int value) = 0;
    virtual QColor getColor(double value) = 0;
    virtual QColor getColor(float value) = 0;
    virtual int getType() = 0;
    virtual int getNumberOfColors() = 0;
    QVector3D getColorVec(int value);
    QVector3D getColorVec(double value);
    QVector3D getColorVec(float value);
    QColor getMorelandColor(QColor c1, QColor c2, float s);

    static const int TYPE_SEQUENTIAL = 0;
    static const int TYPE_DIVERGENT = 1;
    static const int TYPE_QUALITIVE = 2;
    static const int TYPE_RAINBOW = 3;

private:
    int m_Type;
    double m_ValueMin;
    double m_ValueMax;
    QVector<QColor> m_ColorList;
};

#endif // COLORMAP_H
