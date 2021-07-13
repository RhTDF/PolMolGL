#include "filehandler.h"
#include "Data/particle.h"
#include "Data/vertex.h"
#include "Data/globalvars.h"

#include <QTextStream>
#include <QtDebug>
#include <QFileDialog>


FileHandler::FileHandler()
{
    m_SimBox = QVector3D(116.01860, 116.01860, 10.13590);
    m_NumberOfDataFiles = 0;
}

bool FileHandler::loadFile(QString filename)
{

}

bool FileHandler::readXTCFile(QString xtcFile)
{

}

bool FileHandler::readGroFile(QString groFile)
{

}

bool FileHandler::readBinFile(QString binFile)
{
    QVector3D defaultColor(0.5, 0, 0);
    QFile in(binFile);
    int numFrames = 51;
    if (in.open(QIODevice::ReadOnly)) {
        m_VertexList.append(QVector<QVector<Vertex>>());

        qDebug() << "FileHandler:: Data file load. Name: " << binFile;
        QDataStream s(&in);
        s.setFloatingPointPrecision(QDataStream::SinglePrecision);
        int a;
        s >> a;
        s >> a;
        float x,y,z;
        int particleIndex = 0;
        int colindex = 0;
        while (!s.atEnd()) {
            if (m_VertexList.size() == 1) {
                if (particleIndex % 344 == 0) {
                    switch(colindex){
                      case 0:
                        defaultColor = QVector3D(27.0, 158.0, 119.0)/255.0;
                        break;
                      case 1:
                        defaultColor = QVector3D(217.0, 95.0, 2.0)/255.0;
                        break;
                      case 2:
                        defaultColor = QVector3D(117.0, 112.0, 179.0)/255.0;
                        break;
                      case 3:
                        defaultColor = QVector3D(231.0, 41.0, 138.0)/255.0;
                        break;
                      default:
                        defaultColor = QVector3D(0.0, 200.0, 0.0)/255.0;
                        break;
                    }
                    if (colindex < 3) colindex++;
                    else colindex = 0;
                }
            }
            m_VertexList.last().append(QVector<Vertex>());
            for (int i = 0; i < numFrames; i++) {
                s >> x;
                s >> y;
                s >> z;
                m_VertexList.last()[particleIndex].append(Vertex(QVector3D(x,y,z),defaultColor));
            }
            //s.skipRawData(800*3*4);
            particleIndex++;
        }
        DataFile df;
        df.id = m_NumberOfDataFiles++;
        QFileInfo fileinfo(in);
        df.filename = fileinfo.fileName();
        GlobalVars::g_DataFileList.append(df);
        qDebug() << "FileHandler:: Data file read sucessfully. Name: " << df.filename;
        return true;
    }
    return false;
}

bool FileHandler::readStructFile(QString structFile)
{
    QFile in(structFile);
    if (in.open(QIODevice::ReadOnly)) {
        QTextStream s(&in);
        m_ParticleList.append(QVector<Particle>());
        int buffer_index = m_ParticleList.size()-1;
        QStringList list;
        QStringList list2;
        GlobalVars::g_AtomTypeStringList.append(list);
        GlobalVars::g_MoleculeTypeStringList.append(list2);
        qDebug() << "FileHandler::readStructFile() Struct file load. Name: " << structFile;
        while (!s.atEnd()) {
            QString rawLine;
            QStringList splitLine;
            s >> rawLine;
            splitLine = rawLine.split(';');
            Particle p = Particle(splitLine[0].toInt(), splitLine[1], splitLine[2],
                    splitLine[3].toInt());
            m_ParticleList.last().append(p);

            if (!GlobalVars::g_AtomTypeStringList[buffer_index].contains(p.getAtomName())) {
                GlobalVars::g_AtomTypeStringList[buffer_index] << p.getAtomName();
            }
            if (!GlobalVars::g_MoleculeTypeStringList[buffer_index].contains(p.getResName())) {
                GlobalVars::g_MoleculeTypeStringList[buffer_index] << p.getResName();
            }
        }
        qDebug() << "FileHandler::readStructFile() Struct file read sucessfully. Name: " << structFile;
        qDebug() << "FileHandler::readStructFile() Molecule Type List: " << GlobalVars::g_MoleculeTypeStringList;
        qDebug() << "FileHandler::readStructFile() Atome Type List: " << GlobalVars::g_AtomTypeStringList;
        return true;
    }
    return false;
}

QVector3D FileHandler::getSimBox()
{
    return m_SimBox;
}

QVector<QVector<Particle> > FileHandler::getParticleList()
{
    return m_ParticleList;
}

QVector<QVector<QVector<Vertex> > > FileHandler::getVertexList()
{
    return m_VertexList;
}

void FileHandler::setVertexAt(Vertex vertex, int frameIndex, int vertexIndex, int bufferIndex)
{
    m_VertexList[bufferIndex][vertexIndex][frameIndex] = vertex;
}
