#ifndef COLORMAPWIDGET_H
#define COLORMAPWIDGET_H

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

#include "Data/globalvars.h"
#include "ColorMap/colormap.h"
#include "ColorMap/colormapdivergent.h"
#include "ColorMap/colormaprainbow.h"
#include "ColorMap/colormapsequential.h"
#include "ColorMap/colormapcategorical.h"
#include "newcolormapwidget.h"

typedef struct {
   QColor color;
   float position;
   bool isActive;
   bool isHover;
} ColorPreviewRect;

class ColorMapWidget  : public QWidget
{
    Q_OBJECT
public:
    ColorMapWidget(QWidget *parent = nullptr);
    static QIcon drawColorMapIcon(ColorMap *map);
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
public slots:

private slots:
    void newColorMap();
    void cancelNewColorMap();
    void appendColorMap(int type);
    void changeColorMap(QModelIndex index);
private:
    void drawHSLSquare();
    void drawHSLCircle();
    void drawValueSlider();
    void drawColorMapPreview();
    void drawColorMapElementPreview();

    static const int TYPE_SEQUENTIAL = 0;
    static const int TYPE_DIVERGENT = 1;
    static const int TYPE_QUALITIVE = 2;
    static const int TYPE_RAINBOW = 3;

    //QGridLayout *m_MainLayout;
    QHBoxLayout *m_MainLayout;
    QVBoxLayout *m_LeftLayout;
    QVBoxLayout *m_rightLayout;
    QGridLayout *m_ColorPaletteLayout;


    QListView *m_ColorMapListView;
    QStandardItemModel *m_ColorMapListModel;
    QPushButton *m_NewMapBtn;


    ColorMap *m_ColorMap;
    QImage m_MapPreviewImage;
    QLabel *m_MapPreviewLabel;

    QLabel *m_ColorPreviewLabel;

    QImage m_ColorPaletteImage;
    QLabel *m_PaletteLabel;
    QImage m_ValueSliderImage;
    QLabel *m_ValueSliderLabel;
    QColor m_PreviewColor;

    QImage m_ColorMapElementPreviewImage;
    QLabel *m_ColorMapElementPreviewLabel;
    QVector<ColorPreviewRect> m_ColorPreviewRectList;
    ColorPreviewRect m_MidPreviewRect;
    QPoint m_MoveRectOrigin;
    bool m_isMoveRect = false;
    int m_ColorMapType;
    int m_PreviewImageWidth = 800;
    int m_PreviewImageHeight = 75;
    int m_PreviewRectSize = 35;

    QLineEdit *m_RedLineEdit;
    QLineEdit *m_GreenLineEdit;
    QLineEdit *m_BlueLineEdit;
    QLineEdit *m_HueLineEdit;
    QLineEdit *m_SaturationLineEdit;
    QLineEdit *m_LightnessLineEdit;

    NewColorMapWidget *m_NewColorMapWidget;
};

#endif // COLORMAPWIDGET_H
