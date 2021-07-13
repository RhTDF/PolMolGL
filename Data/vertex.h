#ifndef VERTEX_H
#define VERTEX_H


#include <QVector3D>


/**
 * @brief The Vertex class
 */
class Vertex
{
public:
    /**
     * @brief Vertex
     */
    Vertex();
    /**
     * @brief Vertex
     * @param position
     * @param color
     */
    Vertex(QVector3D position, QVector3D color);
    /**
     * @brief Vertex
     * @param position
     */
    Vertex(QVector3D position);
    ~Vertex();
    /**
     * @brief getColor
     * @return
     */
    QVector3D getColor();
    /**
     * @brief setColor
     * @param color
     */
    void setColor(QVector3D color);
    /**
     * @brief getPosition
     * @return
     */
    QVector3D getPosition();
    /**
     * @brief setPosition
     * @param position
     */
    void setPosition(QVector3D position);
    /**
     * @brief colorOffset
     * @return
     */
    static int colorOffset();
    /**
     * @brief posOffset
     * @return
     */
    static int posOffset();
private:
    /**
     * @brief m_Color
     */
    QVector3D m_Color;
    /**
     * @brief m_Position
     */
    QVector3D m_Position;
};

#endif // VERTEX_H
