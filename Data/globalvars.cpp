#include "globalvars.h"
#include "ColorMap/colormapdivergent.h"
#include "ColorMap/colormapcategorical.h"
#include "ColorMap/colormaprainbow.h"
#include "ColorMap/colormapsequential.h"


QVector<DrawQuery> GlobalVars::g_DrawQueryList;
FileHandler *GlobalVars::g_FileHandler;
QVector<DataFile> GlobalVars::g_DataFileList;
QVector<ColorMap *> GlobalVars::g_ColorMapList;
QVector<QStringList> GlobalVars::g_MoleculeTypeStringList;
QVector<QStringList> GlobalVars::g_AtomTypeStringList;
QVector3D GlobalVars::g_SimulationBox;

GlobalVars::GlobalVars()
{

}

bool GlobalVars::init()
{
        initColorMaps();
}

bool GlobalVars::initColorMaps()
{
    QColor blue, red;
    blue.setRgbF(0.230, 0.299, 0.754);
    red.setRgbF(0.706, 0.016, 0.150);
    ColorMapDivergent *morelandBlueRed = new ColorMapDivergent(blue, 0.1f, red, 0.9f, 0.5f);

    QColor purple, orange;
    purple.setRgbF(0.436, 0.308, 0.631);
    orange.setRgbF(0.759, 0.334, 0.046);
    ColorMapDivergent *morelandPurpleOrange = new ColorMapDivergent(purple, 0.1f, orange, 0.9f, 0.5f);

    QColor green;
    green.setRgbF(0.085, 0.532, 0.201);
    ColorMapDivergent *morelandGreenPurple = new ColorMapDivergent(green, 0.1f, purple, 0.9f, 0.5f);

    QColor tan;
    blue.setRgbF(0.217, 0.525, 0.910);
    tan.setRgbF(0.677, 0.492, 0.093);
    ColorMapDivergent *morelandBlueTan = new ColorMapDivergent(blue, 0.1f, tan, 0.9f, 0.5f);

    red.setRgbF(0.758, 0.214, 0.233);
    ColorMapDivergent *morelandGreenRed = new ColorMapDivergent(green, 0.1f, red, 0.9f, 0.5f);

    g_ColorMapList.append(morelandBlueRed);
    g_ColorMapList.append(morelandPurpleOrange);
    g_ColorMapList.append(morelandGreenPurple);
    g_ColorMapList.append(morelandBlueTan);
    g_ColorMapList.append(morelandGreenRed);

    ColorMapCategorical *qualitativeMap1 = new ColorMapCategorical;
    qualitativeMap1->addControlPoint(QColor(202,0,32), 0.0);
    qualitativeMap1->addControlPoint(QColor(244,165,130), 0.2);
    qualitativeMap1->addControlPoint(QColor(247,247,247), 0.4);
    qualitativeMap1->addControlPoint(QColor(146,197,222), 0.6);
    qualitativeMap1->addControlPoint(QColor(5,113,176), 0.8);






    ColorMapCategorical *qualitativeMap2 = new ColorMapCategorical;
    qualitativeMap2->addControlPoint(QColor(228,26,28), 0.0);
    qualitativeMap2->addControlPoint(QColor(55,126,184), 0.25);
    qualitativeMap2->addControlPoint(QColor(77,175,74), 0.5);
    qualitativeMap2->addControlPoint(QColor(152,78,163), 0.75);

    ColorMapCategorical *qualitativeMap3 = new ColorMapCategorical;
    qualitativeMap3->addControlPoint(QColor(228,26,28), 0.0);
    qualitativeMap3->addControlPoint(QColor(55,126,184), 0.125);
    qualitativeMap3->addControlPoint(QColor(77,175,74), 0.25);
    qualitativeMap3->addControlPoint(QColor(152,78,163), 0.375);
    qualitativeMap3->addControlPoint(QColor(255,127,0), 0.5);
    qualitativeMap3->addControlPoint(QColor(255,255,51), 0.625);
    qualitativeMap3->addControlPoint(QColor(166,86,40), 0.75);
    qualitativeMap3->addControlPoint(QColor(247,129,191), 0.875);

    ColorMapCategorical *qualitativeMap4 = new ColorMapCategorical;
    qualitativeMap4->addControlPoint(QColor(166, 206, 227), 0.0);
    qualitativeMap4->addControlPoint(QColor(31,  120, 180), 1.0/19.0);
    qualitativeMap4->addControlPoint(QColor(178, 223, 138), 2.0/19.0);
    qualitativeMap4->addControlPoint(QColor(51,  160, 44 ), 3.0/19.0);
    qualitativeMap4->addControlPoint(QColor(251, 154, 153), 4.0/19.0);
    qualitativeMap4->addControlPoint(QColor(227, 26,  28 ), 5.0/19.0);
    qualitativeMap4->addControlPoint(QColor(253, 191, 111), 6.0/19.0);
    qualitativeMap4->addControlPoint(QColor(255, 127, 0  ), 7.0/19.0);
    qualitativeMap4->addControlPoint(QColor(202, 178, 214), 8.0/19.0);
    qualitativeMap4->addControlPoint(QColor(106, 61,  154), 9.0/19.0);
    qualitativeMap4->addControlPoint(QColor(255, 255, 153), 10.0/19.0);
    qualitativeMap4->addControlPoint(QColor(177, 89,  40 ), 11.0/19.0);
    qualitativeMap4->addControlPoint(QColor(17,  67,  100), 12.0/19.0);
    qualitativeMap4->addControlPoint(QColor(26,  80,  22 ), 13.0/19.0);
    qualitativeMap4->addControlPoint(QColor(98,  11,  11 ), 14.0/19.0);
    qualitativeMap4->addControlPoint(QColor(102, 51,  0  ), 15.0/19.0);
    qualitativeMap4->addControlPoint(QColor(0,   255, 255), 16.0/19.0);
    qualitativeMap4->addControlPoint(QColor(235, 65,  235), 17.0/19.0);
    qualitativeMap4->addControlPoint(QColor(150, 255, 43 ), 18.0/19.0);

    g_ColorMapList.append(qualitativeMap1);
    g_ColorMapList.append(qualitativeMap2);
    g_ColorMapList.append(qualitativeMap3);
    g_ColorMapList.append(qualitativeMap4);

    ColorMapRainbow *rainbowMap = new ColorMapRainbow;

    ColorMapRainbow *rainbowMap2 = new ColorMapRainbow;
    rainbowMap2->setSaturation(1.0);

    ColorMapRainbow *rainbowMap3 = new ColorMapRainbow;
    rainbowMap3->setValue(1.0);

    ColorMapRainbow *rainbowMap4 = new ColorMapRainbow;
    rainbowMap4->setSaturation(1.0);
    rainbowMap4->setValue(1.0);

    g_ColorMapList.append(rainbowMap);
    g_ColorMapList.append(rainbowMap2);
    g_ColorMapList.append(rainbowMap3);
    g_ColorMapList.append(rainbowMap4);

    ColorMapSequential *sequentialMap = new ColorMapSequential;
    sequentialMap->setRightColor(QColor(8,69,148));

    ColorMapSequential *sequentialMap2 = new ColorMapSequential;
    sequentialMap2->setRightColor(QColor(0,90,50));

    ColorMapSequential *sequentialMap3 = new ColorMapSequential;
    sequentialMap3->setRightColor(QColor(74,20,134));

    g_ColorMapList.append(sequentialMap);
    g_ColorMapList.append(sequentialMap2);
    g_ColorMapList.append(sequentialMap3);

    return true;
}
