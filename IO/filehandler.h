#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <QString>
#include <QFile>
#include <QVector>
#include <QVector3D>




class Particle;
class Vertex;

class FileHandler
{
public:
    FileHandler();

    bool loadFile(QString filename);
    bool readXTCFile(QString xtcFile);
    bool readGroFile(QString groFile);
    bool readBinFile(QString binFile);
    bool readStructFile(QString structFile);
    QVector3D getSimBox();
    QVector<QVector<Particle>> getParticleList();
    QVector<QVector<QVector<Vertex>>> getVertexList();
    void setVertexAt(Vertex vertex, int frameIndex, int vertexIndex, int bufferIndex);
//private:
    QVector<QVector<Particle>> m_ParticleList;
    QVector<QVector<QVector<Vertex>>> m_VertexList;
    QVector3D m_SimBox;
    int m_NumberOfDataFiles;


};

#endif // FILEHANDLER_H
