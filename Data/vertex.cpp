#include "vertex.h"

Vertex::Vertex()
{

}

Vertex::Vertex(QVector3D position, QVector3D color)
{
    setPosition(position);
    setColor(color);
}

Vertex::Vertex(QVector3D position)
{
    setPosition(position);
}

Vertex::~Vertex()
{

}

QVector3D Vertex::getColor()
{
    return m_Color;
}

void Vertex::setColor(QVector3D color)
{
    m_Color = color;
}

QVector3D Vertex::getPosition()
{
    return m_Position;
}

void Vertex::setPosition(QVector3D position)
{
    m_Position = position;
}

int Vertex::colorOffset()
{
    return offsetof(Vertex, m_Color);
}

int Vertex::posOffset()
{
    return offsetof(Vertex, m_Position);
}
