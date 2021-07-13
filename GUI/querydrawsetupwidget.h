#ifndef QUERYDRAWSETUPWIDGET_H
#define QUERYDRAWSETUPWIDGET_H

#include <QtMath>
#include <QWidget>
#include <QPainter>
#include <QTimer>
#include <QSpinBox>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QButtonGroup>
#include <QGroupBox>
#include <QSlider>
#include <QComboBox>
#include <QVector>
#include <QColumnView>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QColorDialog>
#include <QRgb>
#include <QListView>
#include <QGridLayout>
#include <QLabel>
#include <QPixmap>
#include <QMouseEvent>
#include <QValidator>
#include <QDoubleSpinBox>
#include <QColorDialog>
#include <QColor>
#include <QStackedLayout>
#include <QRadioButton>

#include "Utility/rangeslider.h"

class ColorMap;

class QueryDrawSetupWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QueryDrawSetupWidget(QWidget *parent = nullptr);

signals:
    void queryChanged();
    void somethingChanged();
    void colorChanged(int buffid);
    void newPolarQuery(int queryindex, int dataindex);
    void depthFilterChanged(int min, int max);
    void depthFilterTypeChanged(int type);
    void displacementFilterChanged(int min, int max);
public slots:
    void addQuery(int query_index);

private slots:
    void changeSelectedQuery(QModelIndex index);
    void pointSizeSpinBoxChanged(double value);
    void pathlineSizeSpinBoxChanged(double value);
    void pointSizeSliderChanged(int value);
    void pathlineSizeSliderChanged(int value);
    void drawContextChanged(int state);
    void drawPointChanged(int state);
    void drawPathChanged(int state);
    void drawNotQueryChanged(int state);
    void changeColorPressed();
    void newQueryPressed();
    void velocityMapHueChanged(int state);

    void colorMapVarChanged(int index);
    void lengthColorChanged();
    void displacementColorChanged();
    void velocityColorChanged();
    void depthColorChanged();
    void curvatureColorChanged();
    void bearingColorChanged();
    void atomColorChanged();
    void residueColorChanged();
    void moleculeColorChanged();
    void clusterColorChanged();

    void selectionTypeChanged(int index);
    void selectionLeftButtonPressed();
    void selectionRightButtonPressed();
    void selectionApplyButtonPressed();

    void filterApplyButtonPressed();

private:

    void setupWidgets();
    void setupQueryList();
    void setupRenderingStyleTab();
    void setupColoringTab();
    void setupSelectionTab();
    void setupFilteringTab();
    QIcon drawColorMapIcon(ColorMap *map);

    void setupLengthColorArea();
    void setupDisplacementColorArea();
    void setupVelocityColorArea();
    void setupDepthColorArea();
    void setupCurvatureColorArea();
    void setupBearingColorArea();
    void setupAtomColorArea();
    void setupResidueColorArea();
    void setupMoleculeColorArea();
    void setupClusterColorArea();

    QHBoxLayout *m_MainLayout;
    QHBoxLayout *m_ColorAreaLayout;
    QTabWidget *m_TabWidget;

    QListView *m_QueryListView;
    QStandardItemModel *m_QueryListModel;

    QSlider *m_PointSizeSlider;
    QSlider *m_PathlineSizeSlider;
    QDoubleSpinBox  *m_PointSizeSpinBox;
    QDoubleSpinBox  *m_PathlineSizeSpinBox;
    QCheckBox *m_RenderContextCheckBox;
    QCheckBox *m_RenderPathCheckBox;
    QCheckBox *m_RenderPointCheckBox;
    QCheckBox *m_NotQueryCheckBox;
    QPushButton *m_ChangeColorBtn;
    QPushButton *m_NewQueryBtn;

    QComboBox *m_ColorMapListComboBox;
    QComboBox *m_ColorMapVarComboBox;
    QCheckBox *m_LocalValueColorCheckBox;

    QWidget *m_VarLengthColorWidget;
    QCheckBox *m_VarLengthInclZCheckBox;

    QWidget *m_VarDispColorWidget;
    QCheckBox *m_VarDispInclZCheckBox;
    QSpinBox *m_VarDispT1SpinBox;
    QSpinBox *m_VarDispT2SpinBox;

    QWidget *m_VarVelocityColorWidget;
    QCheckBox *m_VarVelocityInclZCheckBox;
    QCheckBox *m_VarVelocityMapDirCheckBox;
    QButtonGroup *m_VarVelocityBtnGroup;
    QRadioButton *m_VarVelocityAvgRadioBtn;
    QRadioButton *m_VarVelocityPerStepRadioBtn;

    QWidget *m_VarDepthColorWidget;
    QButtonGroup *m_VarDepthBtnGroup;
    QCheckBox *m_VarDepthCurCheckBox;
    QCheckBox *m_VarDepthStartCheckBox;
    QCheckBox *m_VarDepthEndCheckBox;
    QCheckBox *m_VarDepthChangeCheckBox;

    QWidget *m_VarCurvatureColorWidget;


    QWidget *m_VarBearingColorWidget;


    QWidget *m_VarAtomColorWidget;
    QButtonGroup *m_VarAtomBtnGroup;
    QRadioButton *m_VarAtomSingleHueCheckBox;
    QRadioButton *m_VarAtomMultiHueCheckBox;
    QLabel *m_VarAtomNumClassesLabel;

    QWidget *m_VarResidueColorWidget;
    QButtonGroup *m_VarResidueBtnGroup;
    QRadioButton *m_VarResidueSingleHueCheckBox;
    QRadioButton *m_VarResidueMultiHueCheckBox;
    QLabel *m_VarResidueNumClassesLabel;

    QWidget *m_VarMoleculeColorWidget;
    QButtonGroup *m_VarMoleculeBtnGroup;
    QRadioButton *m_VarMoleculeSingleHueCheckBox;
    QRadioButton *m_VarMoleculeMultiHueCheckBox;
    QLabel *m_VarMoleculeNumClassesLabel;

    QWidget *m_VarClusterColorWidget;

    QListView *m_SelectionInListView;
    QListView *m_SelectionOutListView;
    QStandardItemModel *m_SelectionInModel;
    QStandardItemModel *m_SelectionOutModel;
    QComboBox *m_SelectionTypeComboBox;
    QPushButton *m_SelectionApplyButton;
    QPushButton *m_SelectionRightButton;
    QPushButton *m_SelectionLeftButton;

    QDoubleSpinBox *m_FilterXMinSpinBox;
    QDoubleSpinBox *m_FilterXMaxSpinBox;
    QDoubleSpinBox *m_FilterYMinSpinBox;
    QDoubleSpinBox *m_FilterYMaxSpinBox;
    QDoubleSpinBox *m_FilterZMinSpinBox;
    QDoubleSpinBox *m_FilterZMaxSpinBox;
    QPushButton *m_FilterApplyButton;
    RangeSlider *m_FilterLengthSlider;
    RangeSlider *m_FilterDispSlider;
    RangeSlider *m_FilterDepthSlider;
    QComboBox *m_DepthTypeComboBox;

    QStackedLayout *m_StackedLayout;
};

#endif // QUERYDRAWSETUPWIDGET_H
