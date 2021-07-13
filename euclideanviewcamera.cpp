#include "EuclideanViewCamera.h"
#include <QDebug>


EuclideanViewCamera::EuclideanViewCamera()
{

}

void EuclideanViewCamera::setDefaultView(QMatrix4x4 defaultView)
{
    m_DefaultView = defaultView;
}

void EuclideanViewCamera::setMatrix(QMatrix4x4 transformMatrix)
{
    m_WorldMatrix = transformMatrix;
}

QVector3D EuclideanViewCamera::getTranslation()
{
    return m_Translation;
}

void EuclideanViewCamera::setTranslation(QVector3D translation)
{
    m_Translation = translation;
}

void EuclideanViewCamera::resetTranslation()
{
    setTranslation(QVector3D(0,0,0));
}

void EuclideanViewCamera::translate(QVector3D translate)
{
    setTranslation(m_Translation + translate);
}

QQuaternion EuclideanViewCamera::getRotation()
{
    return m_Rotation;
}

void EuclideanViewCamera::setRotation(QQuaternion rotation)
{
    m_Rotation = rotation;
}

void EuclideanViewCamera::resetRotation()
{
    setRotation(QQuaternion(1,0,0,0));
}

void EuclideanViewCamera::rotate(QQuaternion rotation)
{
    setRotation(rotation * m_Rotation);
}

void EuclideanViewCamera::resetView()
{
    resetTranslation();
    resetRotation();
}

const QVector3D EuclideanViewCamera::FORWARD = QVector3D(0,0,1);
QVector3D EuclideanViewCamera::forward()
{
    return m_Rotation.rotatedVector(FORWARD);
}

const QVector3D EuclideanViewCamera::RIGHT = QVector3D(-1,0,0);
QVector3D EuclideanViewCamera::right()
{
    return m_Rotation.rotatedVector(RIGHT);
}

const QVector3D EuclideanViewCamera::UP = QVector3D(0,1,0);
QVector3D EuclideanViewCamera::up()
{
    return m_Rotation.rotatedVector(UP);
}

QMatrix4x4 EuclideanViewCamera::toMatrix()
{
      m_WorldMatrix.setToIdentity();
      m_WorldMatrix *= m_DefaultView;
      m_WorldMatrix.rotate(m_Rotation.conjugate());
      m_WorldMatrix.translate(-m_Translation);
    return m_WorldMatrix;
}
