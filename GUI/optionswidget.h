#ifndef OPTIONSWIDGET_H
#define OPTIONSWIDGET_H

#include <QtMath>
#include <QWidget>
#include <QGroupBox>
#include <QPushButton>
#include <QPixmap>
#include <QIcon>
#include <QLabel>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QCheckBox>
#include <QRadioButton>
#include <QButtonGroup>
#include <QLineEdit>
#include <QSlider>
#include <QLayout>

class OptionsWidget  : public QWidget
{
    Q_OBJECT
public:
    explicit OptionsWidget(QWidget *parent = nullptr);

signals:
    void backgroundColorChanged(QColor color);
    void resetCameraPressed();
    void euclideanAAChanged(int state);

    void polarGUISizeChanged(int value);
    void polarTrajectorySizeChanged(int value);
    void drawGUIChanged(int state);
    void drawGridChanged(int state);
    void drawContextChanged(int state);
    void lockAngleChanged(int state);
    void lengthFilterTypeChanged(bool checked);
    void polarAAChanged(int state);
    void flatPerspectiveChanged(int state);

public slots:
    void changeAngleFilters(double min, double max);

private slots:
    void backgroundColorButtonPressed();


private:
    void setupWidget();
    void setupEuclideanGroupBox();
    void setupPolarGroupBox();
    void setupScreenshotGroupBox();


    QGroupBox *m_EuclideanGroupBox;
    QPushButton *m_ResetCameraBtn;
    QPushButton *m_BGColorBtn;
    QColor m_BGColor;
    QCheckBox *m_EuclideanAACheckBox;

    QGroupBox *m_PolarGroupBox;
    QDoubleSpinBox *m_MinAngleSpinBox;
    QDoubleSpinBox *m_MaxAngleSpinBox;
    QCheckBox *m_LockAngleCheckBox;
    QButtonGroup *m_LengthFilterBtnGroup;
    QRadioButton *m_MaxLengthRadioBtn;
    QRadioButton *m_MinLengthRadioBtn;
    QSlider *m_PolarLineSizeSlider;
    QSlider *m_PolarGUISizeSlider;
    QCheckBox *m_ShowCircleCheckBox;
    QCheckBox *m_ShowOriginCheckBox;
    QCheckBox *m_ShowContextCheckBox;
    QCheckBox *m_PolarAACheckBox;
    QCheckBox *m_FlatPerspectiveCheckBox;

    QGroupBox *m_ScreenshotGroupBox;
    QButtonGroup *m_ScreenshotBtnGroup;
    QRadioButton *m_PolarRadioBtn;
    QRadioButton *m_EuclidRadioBtn;
    QLineEdit *m_FileNameLineEdit;
    QSpinBox *m_WidthSpinBox;
    QSpinBox *m_HeightSpinBox;
    QPushButton *m_takeScreenshotBtn;

};

#endif // OPTIONSWIDGET_H
