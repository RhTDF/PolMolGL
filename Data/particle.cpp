#include "particle.h"
#include "globalvars.h"
#include <QDebug>


Particle::Particle()
{

}

Particle::Particle(unsigned int resID, QString resName,
                   QString atomName, unsigned int atomID)
{
    setResID(resID);
    setResName(resName);
    setAtomName(atomName);
    setAtomID(atomID);
    m_VertexList = QVector<Vertex>();
}

Particle::Particle(unsigned int resID, QString resName, QString atomName,
                   unsigned int atomID, QVector<Vertex> vertexVector)
{
    setResID(resID);
    setResName(resName);
    setAtomName(atomName);
    setAtomID(atomID);
    m_VertexList =vertexVector;
}

unsigned int Particle::getResID()
{
    return m_ResidueID;
}

QString Particle::getResName()
{
    return m_ResidueName;
}

QString *Particle::getResNamePtr()
{
    return &m_ResidueName;
}

QString Particle::getAtomName()
{
    return m_AtomName;
}

unsigned int Particle::getAtomID()
{
    return m_AtomID;
}

QVector<Vertex> Particle::getPosVector()
{
    return m_VertexList;
}

Vertex Particle::getPosVector(int time)
{
    return m_VertexList.at(time);
}


void Particle::setResID(unsigned int id)
{
    m_ResidueID = id;
}

void Particle::setResName(QString name)
{
    m_ResidueName = name;
}

void Particle::setAtomName(QString name)
{
    m_AtomName = name;
}

void Particle::setAtomID(unsigned int id)
{
    m_AtomID = id;
}

int Particle::getResidueNameIndex(int buffer_id)
{
    int residue_index = GlobalVars::g_MoleculeTypeStringList[buffer_id].indexOf(m_ResidueName);
    return residue_index;
}

void Particle::setParticleDisplacement(float displacement)
{
    m_ParticleDisplacement = displacement;
}

float Particle::getParticleDisplacement()
{
    return m_ParticleDisplacement;
}
