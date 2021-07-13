#include "optionswidget.h"

#include <QDebug>
#include <QColorDialog>

OptionsWidget::OptionsWidget(QWidget *parent)
{
    setupWidget();
}

void OptionsWidget::changeAngleFilters(double min, double max)
{
    m_MinAngleSpinBox->setValue(min*180.0/M_PI);
    m_MaxAngleSpinBox->setValue(max*180.0/M_PI);
}

void OptionsWidget::backgroundColorButtonPressed()
{
    QPixmap bgColorIcon(50,50);
    QColor iconColor = QColorDialog::getColor(m_BGColor);
    if (iconColor.isValid()) {
        bgColorIcon.fill(iconColor);
        m_BGColor = iconColor;
        m_BGColorBtn->setIcon(QIcon(bgColorIcon));
        emit backgroundColorChanged(m_BGColor);
    }
}

void OptionsWidget::setupWidget()
{
    qDebug() << "OptionsWidget::setupWidget() - setupEuclideanGroupBox();";
    setupEuclideanGroupBox();
    qDebug() << "OptionsWidget::setupWidget() - setupPolarGroupBox();";
    setupPolarGroupBox();
    qDebug() << "OptionsWidget::setupWidget() - setupScreenshotGroupBox();";
    setupScreenshotGroupBox();

    qDebug() << "OptionsWidget::setupWidget() - mainLayout";

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(m_EuclideanGroupBox);
    mainLayout->addWidget(m_PolarGroupBox);
    mainLayout->addWidget(m_ScreenshotGroupBox);

    setLayout(mainLayout);

    qDebug() << "OptionsWidget::setupWidget() - mainLayout";
}

void OptionsWidget::setupEuclideanGroupBox()
{
    m_EuclideanGroupBox = new QGroupBox("Euclidean View Settings");
    m_ResetCameraBtn = new QPushButton(QIcon(QPixmap(":/icons/camera.png")), "Reset Camera");
    QPixmap bgColorIcon(50,50);
    m_BGColor = QColor(Qt::black);
    bgColorIcon.fill(m_BGColor);
    m_BGColorBtn = new QPushButton(QIcon(bgColorIcon), "Background Color");
    m_EuclideanAACheckBox = new QCheckBox("Anti-Alias pathlines:");
    m_EuclideanAACheckBox->setCheckState(Qt::Unchecked);

    QVBoxLayout *gbLayout = new QVBoxLayout;
    gbLayout->addWidget(m_ResetCameraBtn);
    gbLayout->addWidget(m_BGColorBtn);
    gbLayout->addWidget(m_EuclideanAACheckBox);
    m_EuclideanGroupBox->setLayout(gbLayout);

    connect(m_ResetCameraBtn, SIGNAL(released()), this, SIGNAL(resetCameraPressed()));
    connect(m_BGColorBtn, SIGNAL(released()), this, SLOT(backgroundColorButtonPressed()));
    connect(m_EuclideanAACheckBox, SIGNAL(stateChanged(int)), this, SIGNAL(euclideanAAChanged(int)));

}

void OptionsWidget::setupPolarGroupBox()
{
    m_PolarGroupBox = new QGroupBox("Polar View Settings");

    m_MinAngleSpinBox = new QDoubleSpinBox;
    m_MaxAngleSpinBox = new QDoubleSpinBox;
    m_MinAngleSpinBox->setRange(-180.0,180.0);
    m_MaxAngleSpinBox->setRange(-180.0,180.0);

    m_LockAngleCheckBox = new QCheckBox("Lock Angle: ");
    m_LockAngleCheckBox->setLayoutDirection(Qt::RightToLeft);
    connect(m_LockAngleCheckBox, SIGNAL(stateChanged(int)), this, SIGNAL(lockAngleChanged(int)));

    QHBoxLayout *hbox1 = new QHBoxLayout;
    hbox1->addWidget(new QLabel("Minimum Angle: "));
    hbox1->addWidget(m_MinAngleSpinBox);
    hbox1->addWidget(new QLabel("Max: "));
    hbox1->addWidget(m_MaxAngleSpinBox);
    hbox1->addWidget(m_LockAngleCheckBox);
    hbox1->addStretch();


    m_MaxLengthRadioBtn = new QRadioButton("Max: ");
    m_MaxLengthRadioBtn->setLayoutDirection(Qt::RightToLeft);
    m_MinLengthRadioBtn = new QRadioButton("Min: ");
    m_MinLengthRadioBtn->setLayoutDirection(Qt::RightToLeft);
    m_MaxLengthRadioBtn->setChecked(true);
    connect(m_MaxLengthRadioBtn, SIGNAL(toggled(bool)), this, SIGNAL(lengthFilterTypeChanged(bool)));
    m_LengthFilterBtnGroup = new QButtonGroup;
    m_LengthFilterBtnGroup->addButton(m_MaxLengthRadioBtn);
    m_LengthFilterBtnGroup->addButton(m_MinLengthRadioBtn);

    QHBoxLayout *hbox2 = new QHBoxLayout;
    hbox2->addWidget(new QLabel("Filter Path Displacement "));
    hbox2->addWidget(m_MaxLengthRadioBtn);
    hbox2->addWidget(m_MinLengthRadioBtn);
    hbox2->addStretch();

    m_PolarLineSizeSlider = new QSlider(Qt::Horizontal);
    m_PolarLineSizeSlider->setMinimum(0);
    m_PolarLineSizeSlider->setMaximum(1000);
    m_PolarLineSizeSlider->setTickPosition(QSlider::TicksBothSides);
    m_PolarLineSizeSlider->setTickInterval(100);
    m_PolarLineSizeSlider->setValue(100);
    connect(m_PolarLineSizeSlider, SIGNAL(valueChanged(int)), this, SIGNAL(polarTrajectorySizeChanged(int)));

    QHBoxLayout *hbox3 = new QHBoxLayout;
    hbox3->addWidget(new QLabel("Trajectory Line Thickness: "));
    hbox3->addWidget(m_PolarLineSizeSlider);

    m_PolarGUISizeSlider = new QSlider(Qt::Horizontal);
    m_PolarGUISizeSlider->setMinimum(0);
    m_PolarGUISizeSlider->setMaximum(1000);
    m_PolarGUISizeSlider->setTickPosition(QSlider::TicksBothSides);
    m_PolarGUISizeSlider->setTickInterval(100);
    m_PolarGUISizeSlider->setValue(400);
    connect(m_PolarGUISizeSlider, SIGNAL(valueChanged(int)), this, SIGNAL(polarGUISizeChanged(int)));

    QHBoxLayout *hbox4 = new QHBoxLayout;
    hbox4->addWidget(new QLabel("GUI size: "));
    hbox4->addWidget(m_PolarGUISizeSlider);

    m_ShowCircleCheckBox = new QCheckBox("Show Circle: ");
    m_ShowOriginCheckBox = new QCheckBox("Show Origin: ");
    m_ShowContextCheckBox = new QCheckBox("Show Context");
    m_PolarAACheckBox = new QCheckBox("Anti-Alias pathlines:");
    m_FlatPerspectiveCheckBox = new QCheckBox("Flatten perspective");

    m_ShowCircleCheckBox->setCheckState(Qt::Checked);
    m_ShowOriginCheckBox->setCheckState(Qt::Unchecked);
    m_ShowContextCheckBox->setCheckState(Qt::Unchecked);
    m_PolarAACheckBox->setCheckState(Qt::Unchecked);
    m_FlatPerspectiveCheckBox->setCheckState(Qt::Unchecked);

    m_ShowCircleCheckBox->setLayoutDirection(Qt::RightToLeft);
    m_ShowOriginCheckBox->setLayoutDirection(Qt::RightToLeft);
    m_ShowContextCheckBox->setLayoutDirection(Qt::RightToLeft);   
    m_PolarAACheckBox->setLayoutDirection(Qt::RightToLeft);
    m_FlatPerspectiveCheckBox->setLayoutDirection(Qt::RightToLeft);

    connect(m_ShowCircleCheckBox, SIGNAL(stateChanged(int)), this, SIGNAL(drawGUIChanged(int)));
    connect(m_ShowOriginCheckBox, SIGNAL(stateChanged(int)), this, SIGNAL(drawGridChanged(int)));
    connect(m_ShowContextCheckBox, SIGNAL(stateChanged(int)), this, SIGNAL(drawContextChanged(int)));
    connect(m_PolarAACheckBox, SIGNAL(stateChanged(int)), this, SIGNAL(polarAAChanged(int)));
    connect(m_FlatPerspectiveCheckBox, SIGNAL(stateChanged(int)), this, SIGNAL(flatPerspectiveChanged(int)));

    QHBoxLayout *hbox5 = new QHBoxLayout;
    hbox5->addWidget(m_ShowCircleCheckBox);
    hbox5->addWidget(m_ShowOriginCheckBox);
    hbox5->addWidget(m_ShowContextCheckBox);
    hbox5->addWidget(m_PolarAACheckBox);
    hbox5->addWidget(m_FlatPerspectiveCheckBox);
    hbox5->addStretch();

    QVBoxLayout *gbLayout = new QVBoxLayout;
    gbLayout->addLayout(hbox1);
    gbLayout->addLayout(hbox2);
    gbLayout->addLayout(hbox3);
    gbLayout->addLayout(hbox4);
    gbLayout->addLayout(hbox5);
    m_PolarGroupBox->setLayout(gbLayout);

}

void OptionsWidget::setupScreenshotGroupBox()
{
    m_ScreenshotGroupBox = new QGroupBox("Screenshot settings");

    m_PolarRadioBtn = new QRadioButton("Polar View");
    m_EuclidRadioBtn = new QRadioButton("Euclidean View");
    m_ScreenshotBtnGroup = new QButtonGroup;
    m_ScreenshotBtnGroup->addButton(m_PolarRadioBtn);
    m_ScreenshotBtnGroup->addButton(m_EuclidRadioBtn);
    QHBoxLayout *hbox1 = new QHBoxLayout;
    hbox1->addWidget(m_PolarRadioBtn);
    hbox1->addWidget(m_EuclidRadioBtn);

    m_FileNameLineEdit = new QLineEdit;
    QHBoxLayout *hbox2 = new QHBoxLayout;
    hbox2->addWidget(new QLabel("Filename: "));
    hbox2->addWidget(m_FileNameLineEdit);

    m_WidthSpinBox = new QSpinBox;
    m_HeightSpinBox = new QSpinBox;
    QHBoxLayout *hbox3 = new QHBoxLayout;
    hbox3->addWidget(new QLabel("Resolution - Width: "));
    hbox3->addWidget(m_WidthSpinBox);
    hbox3->addWidget(new QLabel("Height: "));
    hbox3->addWidget(m_HeightSpinBox);

    m_takeScreenshotBtn = new QPushButton("Render");

    QVBoxLayout *gbLayout = new QVBoxLayout;
    gbLayout->addLayout(hbox1);
    gbLayout->addLayout(hbox2);
    gbLayout->addLayout(hbox3);
    gbLayout->addWidget(m_takeScreenshotBtn);

    m_ScreenshotGroupBox->setLayout(gbLayout);
}
