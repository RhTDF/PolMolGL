#ifndef CAMERA3D_H
#define CAMERA3D_H

#include <QVector3D>
#include <QMatrix4x4>
#include <QQuaternion>

class EuclideanViewCamera
{
public:
    EuclideanViewCamera();
    void setDefaultView(QMatrix4x4 defaultView);
    void setMatrix(QMatrix4x4 transformMatrix);
    QVector3D getTranslation();
    void setTranslation(QVector3D translation);
    void resetTranslation();
    void translate(QVector3D translate);
    QQuaternion getRotation();
    void setRotation(QQuaternion rotation);
    void resetRotation();
    void rotate(QQuaternion rotation);
    void resetView();
    QVector3D forward();
    QVector3D right();
    QVector3D up();
    QMatrix4x4 toMatrix();

    static const QVector3D FORWARD;
    static const QVector3D RIGHT;
    static const QVector3D UP;
private:
    QMatrix4x4 m_WorldMatrix;
    QMatrix4x4 m_DefaultView;
    QVector3D m_Translation;
    QQuaternion m_Rotation;


};

#endif // CAMERA3D_H


