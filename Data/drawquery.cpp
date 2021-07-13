#include "drawquery.h"

DrawQuery::DrawQuery()
{
    m_isDrawPath = true;
    m_isDrawParticle = false;
    m_isNotQuery = false;
    m_ParticlePointSize = 1.0f;
    m_PathLineThickness = 1.0f;
}

QBitArray DrawQuery::getBitArray()
{
    return m_BitArray;
}

void DrawQuery::setBitArray(QBitArray arr)
{
    m_BitArray = arr;
    m_AttributeBitArray = QBitArray(arr.size(), true);
    m_PolarBitArray = QBitArray(arr.size(), true);
    m_AuxBitArray = QBitArray(arr.size(), true);
}

void DrawQuery::setBit(int index, int bit)
{
    m_BitArray.setBit(index, bit==1);
}

void DrawQuery::setBit(int index, bool bit)
{
    m_BitArray.setBit(index, bit);
}

bool DrawQuery::getBit(int index)
{
    bool bit = m_BitArray.at(index) && m_AttributeBitArray.at(index) && m_PolarBitArray.at(index) && m_AuxBitArray.at(index);
    return bit;
}

bool DrawQuery::getPolarContextBit(int index)
{
    bool bit = m_BitArray.at(index) && m_AttributeBitArray.at(index) && !m_PolarBitArray.at(index) && m_AuxBitArray.at(index);
    return bit;
}

void DrawQuery::setAttribBit(int index, bool bit)
{
    m_AttributeBitArray.setBit(index, bit);
}

void DrawQuery::setPolarBit(int index, bool bit)
{
    m_PolarBitArray.setBit(index, bit);
}

void DrawQuery::setAuxBit(int index, bool bit)
{
    m_AuxBitArray.setBit(index, bit);
}

bool DrawQuery::isDrawPath()
{
    return m_isDrawPath;
}

bool DrawQuery::isDrawParticle()
{
    return m_isDrawParticle;
}

bool DrawQuery::isContext()
{
    return m_isContext;
}

bool DrawQuery::isNotQuery()
{
    return m_isNotQuery;
}

int DrawQuery::getParentQueryIndex()
{
    return m_parentQueryIndex;
}

float DrawQuery::getPathlineThickness()
{
    return m_PathLineThickness;
}

float DrawQuery::getParticlePointSize()
{
    return m_ParticlePointSize;
}

void DrawQuery::setDrawPath(bool b)
{
    m_isDrawPath = b;
}

void DrawQuery::setDrawParticle(bool b)
{
    m_isDrawParticle = b;
}

void DrawQuery::setContext(bool b)
{
    m_isContext = b;
}

void DrawQuery::setIsNotQuery(bool b)
{
    m_isNotQuery = b;
}

void DrawQuery::setParentQueryIndex(int index)
{
    m_parentQueryIndex = index;
}

void DrawQuery::setPathlineThickness(float x)
{
    m_PathLineThickness = x;
}

void DrawQuery::setParticlePointSize(float x)
{
    m_ParticlePointSize = x;
}

void DrawQuery::setQueryName(QString name)
{
    m_QueryName = name;
}

QString DrawQuery::getQueryName()
{
    return m_QueryName;
}

int DrawQuery::getColorMappingVar()
{
    return m_ColorMappingVar;
}


double DrawQuery::getColorMapMaxVal()
{
    return m_MaxColorMappingVarVal;
}

double DrawQuery::getColorMapMinVal()
{
    return m_MinColorMappingVarVal;
}

void DrawQuery::setColorMappingVar(int var)
{
    m_ColorMappingVar = var;
}

void DrawQuery::setColorMapMaxVal(double val)
{
    m_MaxColorMappingVarVal = val;
}

void DrawQuery::setColorMapMinVal(double val)
{
    m_MinColorMappingVarVal = val;
}

void DrawQuery::setBufferIndex(int b)
{
    m_dataBufferIndex = b;
}

int DrawQuery::getDataBufferIndex()
{
    return m_dataBufferIndex;
}

int DrawQuery::getQueryType()
{
    return m_QueryType;
}

void DrawQuery::setQueryType(int type)
{
    m_QueryType = type;
}
