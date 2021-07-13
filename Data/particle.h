#ifndef PARTICLE_H
#define PARTICLE_H


#include <QString>
#include <QVector3D>
#include <QVector>

#include "vertex.h"


/**
 * @brief The Particle class
 */
class Particle
{
public:
    /**
     * @brief Particle
     */
    Particle();
    /**
     * @brief Particle
     * @param resID
     * @param resName
     * @param atomName
     * @param atomID
     */
    Particle(unsigned int resID, QString resName, QString atomName,
             unsigned int atomID);
    /**
     * @brief Particle
     * @param resID
     * @param resName
     * @param atomName
     * @param atomID
     * @param vertexVector
     */
    Particle(unsigned int resID, QString resName, QString atomName,
             unsigned int atomID, QVector<Vertex> vertexVector);
    /**
     * @brief getResID
     * @return
     */
    unsigned int getResID();
    /**
     * @brief getResName
     * @return
     */
    QString getResName();
    /**
     * @brief getAtomName
     * @return
     */
    QString *getResNamePtr();
    QString getAtomName();
    /**
     * @brief getAtomID
     * @return
     */
    unsigned int getAtomID();
    /**
     * @brief getPosVector
     * @return
     */
    QVector<Vertex> getPosVector();
    /**
     * @brief getPosVector
     * @param time
     * @return
     */
    Vertex getPosVector(int time);
    /**
     * @brief setResID
     * @param id
     */
    void setResID(unsigned int id);
    /**
     * @brief setResName
     * @param name
     */
    void setResName(QString name);
    /**
     * @brief setAtomName
     * @param name
     */
    void setAtomName(QString name);
    /**
     * @brief setAtomID
     * @param id
     */
    void setAtomID(unsigned int id);

    int getResidueNameIndex(int buffer_id);

    void setParticleDisplacement(float displacement);
    float getParticleDisplacement();
private:
    /**
     * @brief m_ResidueID
     */
    unsigned int m_ResidueID;
    /**
     * @brief m_AtomID
     */
    unsigned int m_AtomID;
    /**
     * @brief m_VertexList
     */
    QVector<Vertex> m_VertexList;
    /**
     * @brief m_ResidueName
     */
    QString m_ResidueName;
    /**
     * @brief m_AtomName
     */
    QString m_AtomName;

    float m_ParticleDisplacement;
};

#endif // PARTICLE_H
