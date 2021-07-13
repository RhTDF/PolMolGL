#ifndef DRAWQUERY_H
#define DRAWQUERY_H

#include <QBitArray>
#include <QString>
#include <QStringList>

/*
 * QUERY TYPE
 * 0 = base
 * 1 = ATOM NAME
 * 2 = RESIDUE NAME
 *
*/


class DrawQuery
{
public:
    DrawQuery();

    QBitArray getBitArray();
    void setBitArray(QBitArray arr);
    void setBit(int index, int bit);
    void setBit(int index, bool bit);
    bool getBit(int index);
    bool getPolarContextBit(int index);

    void setAttribBit(int index, bool bit);
    void setPolarBit(int index, bool bit);
    void setAuxBit(int index, bool bit);

    bool isDrawPath();
    bool isDrawParticle();
    bool isContext();
    bool isNotQuery();
    int getParentQueryIndex();
    float getPathlineThickness();
    float getParticlePointSize();


    void setDrawPath(bool b);
    void setDrawParticle(bool b);
    void setContext(bool b);
    void setIsNotQuery(bool b);
    void setParentQueryIndex(int index);
    void setPathlineThickness(float x);
    void setParticlePointSize(float x);
    void setQueryName(QString name);
    QString getQueryName();
    int getColorMappingVar();
    double getColorMapMaxVal();
    double getColorMapMinVal();
    void setColorMappingVar(int var);
    void setColorMapMaxVal(double val);
    void setColorMapMinVal(double val);

    void setBufferIndex(int b);
    int getDataBufferIndex();

    int getQueryType();
    void setQueryType(int type);

private:
    QBitArray m_BitArray;
    QBitArray m_AttributeBitArray;
    QBitArray m_PolarBitArray;
    QBitArray m_AuxBitArray;

    bool m_isDrawPath;
    bool m_isDrawParticle;
    bool m_isNotQuery;
    bool m_isContext;
    int m_parentQueryIndex;
    int m_dataBufferIndex;
    QString m_QueryName;
    double m_MaxColorMappingVarVal;
    double m_MinColorMappingVarVal;
    int m_QueryType;
    int m_ColorMappingVar;


    float m_PathLineThickness;
    float m_ParticlePointSize;
};

#endif // DRAWQUERY_H
