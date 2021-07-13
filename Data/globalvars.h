#ifndef GLOBALVARS_H
#define GLOBALVARS_H

#include <QVector>
#include "drawquery.h"
#include "IO/filehandler.h"

class QColor;
class Particle;
class ColorMap;

typedef struct {
   int id;
   QString filename;
} DataFile;

class GlobalVars
{
public:
    GlobalVars();
    static bool init();

    static QColor g_ContextColor;
    static QColor g_BackgroundColor;
    static double g_ContextLineSize;
    static double g_ContextPointSize;
    static double g_SelectionLineSize;
    static double g_SelectionPointSize;
    static double g_SciViewFOV;
    static double g_SciViewCullDistance;
    static double g_PlaybackSpeed;
    static int g_SimulationSpan;
    static int g_NumberOfLipidParticles;
    static int g_NumberOfProteinParticles;
    static bool g_ShowContext;
    static bool g_IsPlayback;
    static QVector<QStringList> g_MoleculeTypeStringList;
    static QVector<QStringList> g_AtomTypeStringList;
    static FileHandler *g_FileHandler;
    static QVector<DataFile> g_DataFileList;
    static QVector<DrawQuery> g_DrawQueryList;
    static QVector<Particle *> g_SelectedLipidList;
    static QVector<Particle *> g_SelectedProteinList;
    static QVector<Particle *> g_ContextLipidList;
    static QVector<Particle *> g_ContextProteinList;
    static QVector<ColorMap *> g_ColorMapList;
    static QVector3D g_SimulationBox;

private:
    static bool initColorMaps();

};

#endif // GLOBALVARS_H
