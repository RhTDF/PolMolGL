#include "colormapwidget.h"
#include <qdebug.h>
ColorMapWidget::ColorMapWidget(QWidget *parent)
{
    setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum));
    setMouseTracking(true);

    m_MainLayout = new QHBoxLayout;
    m_LeftLayout = new QVBoxLayout;
    m_rightLayout = new QVBoxLayout;
    m_ColorPaletteLayout = new QGridLayout;
    m_MainLayout->setSizeConstraint(QLayout::SetMinAndMaxSize);
    m_MainLayout->addLayout(m_LeftLayout);
    m_MainLayout->addLayout(m_rightLayout);
    m_rightLayout->addLayout(m_ColorPaletteLayout);


    m_ColorMapListView = new QListView;
    m_ColorMapListModel = new QStandardItemModel;
    QStandardItem *item;
    for (int i = 0; i < GlobalVars::g_ColorMapList.size(); i++) {
        if (GlobalVars::g_ColorMapList[i]->getType() == TYPE_DIVERGENT) {

            qDebug() << 1;

            qDebug() << "2";
            item = new QStandardItem(drawColorMapIcon(GlobalVars::g_ColorMapList[i]), "Divergent Map " + QString::number(i));

        } else if (GlobalVars::g_ColorMapList[i]->getType() == TYPE_RAINBOW) {
            item = new QStandardItem("Rainbow Map " + QString::number(i));
        } else if (GlobalVars::g_ColorMapList[i]->getType() == TYPE_QUALITIVE) {
            item = new QStandardItem("Categorical Map " + QString::number(i));
        } else if (GlobalVars::g_ColorMapList[i]->getType() == TYPE_SEQUENTIAL) {
            item = new QStandardItem("Sequential Map " + QString::number(i));
        }
        m_ColorMapListModel->appendRow(item);
    }
    m_ColorMapListView->setModel(m_ColorMapListModel);
    connect(m_ColorMapListView, SIGNAL(clicked(QModelIndex)),
            this, SLOT(changeColorMap(QModelIndex)));

    m_NewColorMapWidget = new NewColorMapWidget;

    m_NewMapBtn = new QPushButton("New Map");
    connect(m_NewMapBtn, SIGNAL(clicked()),
            this, SLOT(newColorMap()));

    connect(m_NewColorMapWidget, SIGNAL(noPressed()),
            this, SLOT(cancelNewColorMap()));

    connect(m_NewColorMapWidget, SIGNAL(okPressed(int)),
            this, SLOT(appendColorMap(int)));

    m_LeftLayout->addWidget(m_ColorMapListView);
    m_LeftLayout->addWidget(m_NewMapBtn);

    //drawHSLSquare();
    drawHSLCircle();
    m_PaletteLabel = new QLabel;
    m_PaletteLabel->setPixmap(QPixmap::fromImage(m_ColorPaletteImage));
    m_PaletteLabel->setFixedSize(120,120);
    QIntValidator *lineEditValidator = new QIntValidator(0,255);
    m_RedLineEdit = new QLineEdit("0");
    m_GreenLineEdit  = new QLineEdit("0");
    m_BlueLineEdit = new QLineEdit("0");
    m_HueLineEdit = new QLineEdit("0");
    m_SaturationLineEdit = new QLineEdit("0");
    m_LightnessLineEdit = new QLineEdit("0");
    m_RedLineEdit->setValidator(lineEditValidator);
    m_GreenLineEdit->setValidator(lineEditValidator);
    m_BlueLineEdit->setValidator(lineEditValidator);
    m_HueLineEdit->setValidator(lineEditValidator);
    m_SaturationLineEdit->setValidator(lineEditValidator);
    m_LightnessLineEdit->setValidator(lineEditValidator);
    m_RedLineEdit->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    m_GreenLineEdit->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    m_BlueLineEdit->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    m_HueLineEdit->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    m_SaturationLineEdit->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    m_LightnessLineEdit->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    m_RedLineEdit->setFixedWidth(40);
    m_GreenLineEdit->setFixedWidth(40);
    m_BlueLineEdit->setFixedWidth(40);
    m_HueLineEdit->setFixedWidth(40);
    m_SaturationLineEdit->setFixedWidth(40);
    m_LightnessLineEdit->setFixedWidth(40);



    m_ColorPreviewLabel = new QLabel(this);
    QImage colorPreviewImage = QImage(32,32,QImage::Format_RGB32);
    colorPreviewImage.fill(QColor(0,0,0));
    m_ColorPreviewLabel->setPixmap(QPixmap::fromImage(colorPreviewImage));
    m_ValueSliderLabel = new QLabel;
    //m_ValueSliderLabel->setMouseTracking(true);
    drawValueSlider();
    m_ValueSliderLabel->setPixmap(QPixmap::fromImage(m_ValueSliderImage));

    m_ColorMapType = TYPE_DIVERGENT;
    m_ColorMapElementPreviewLabel = new QLabel;

    m_ColorMapElementPreviewLabel->setMouseTracking(true);
    QColor c1, c2;
    c1.setRgbF(0.230, 0.299, 0.754);
    c2.setRgbF(0.706, 0.016, 0.150);
    ColorPreviewRect r1, r2;
    r1.color = c1;
    r1.position = 0.0f;
    r1.isActive = false;
    r2.color = c2;
    r2.position = 1.0f;
    r2.isActive = true;
    m_ColorMap = new ColorMapDivergent(c1, 0.0f, c2, 1.0f, 0.5f);
    m_MidPreviewRect.color = Qt::white;
    m_MidPreviewRect.position = 0.5f;
    m_MidPreviewRect.isActive = false;
    m_ColorPreviewRectList.append(r1);
    m_ColorPreviewRectList.append(r2);
    drawColorMapElementPreview();

    m_MapPreviewLabel = new QLabel;
    drawColorMapPreview();
    m_MapPreviewLabel->setPixmap(QPixmap::fromImage(m_MapPreviewImage));

    m_ColorPaletteLayout->addWidget(m_PaletteLabel, 0, 0, 4, 1);
    m_ColorPaletteLayout->addWidget(m_ValueSliderLabel, 0, 1, 4, 1);
    m_ColorPaletteLayout->addWidget(new QLabel("Hue:"),0,2);
    m_ColorPaletteLayout->addWidget(new QLabel("Saturation:"),1,2);
    m_ColorPaletteLayout->addWidget(new QLabel("Value:"),2,2);
    m_ColorPaletteLayout->addWidget(m_HueLineEdit,0,3);
    m_ColorPaletteLayout->addWidget(m_SaturationLineEdit,1,3);
    m_ColorPaletteLayout->addWidget(m_LightnessLineEdit,2,3);
    m_ColorPaletteLayout->addWidget(new QLabel("Red:"),0,4);
    m_ColorPaletteLayout->addWidget(new QLabel("Green:"),1,4);
    m_ColorPaletteLayout->addWidget(new QLabel("Blue:"),2,4);
    m_ColorPaletteLayout->addWidget(m_RedLineEdit,0,5);
    m_ColorPaletteLayout->addWidget(m_GreenLineEdit,1,5);
    m_ColorPaletteLayout->addWidget(m_BlueLineEdit,2,5);
    m_ColorPaletteLayout->addWidget(m_ColorPreviewLabel,3,2);
    m_ColorPaletteLayout->setSizeConstraint(QLayout::SetMinAndMaxSize);
    m_ColorPaletteLayout->addItem(new QSpacerItem(1,1,QSizePolicy::Expanding),0,6);
    m_rightLayout->addWidget(m_ColorMapElementPreviewLabel);
    m_rightLayout->addWidget(m_MapPreviewLabel);


    setLayout(m_MainLayout);

}

void ColorMapWidget::mousePressEvent(QMouseEvent *event)
{
    if (m_PaletteLabel->rect().contains(m_PaletteLabel->mapFrom(this, event->pos()))) {
        QColor mouseColor = QWidget::grab().toImage().pixelColor(event->pos());
        qDebug() << mouseColor;
        m_RedLineEdit->setText(QString::number(mouseColor.red()));
        m_GreenLineEdit->setText(QString::number(mouseColor.green()));
        m_BlueLineEdit->setText(QString::number(mouseColor.blue()));
        m_HueLineEdit->setText(QString::number(mouseColor.hue()));
        m_SaturationLineEdit->setText(QString::number(mouseColor.saturation()));
        m_LightnessLineEdit->setText(QString::number(mouseColor.value()));
        QImage colorPreviewImage = QImage(32,32,QImage::Format_RGB32);
        colorPreviewImage.fill(mouseColor);
        m_ColorPreviewLabel->setPixmap(QPixmap::fromImage(colorPreviewImage));
        m_PreviewColor = mouseColor;
        drawValueSlider();
        for (auto& rect : m_ColorPreviewRectList) {
            if (rect.isActive) {
                rect.color = mouseColor;

            }
        }
        ColorMapDivergent *map = &dynamic_cast<ColorMapDivergent&>(*m_ColorMap);
        map->setColors(m_ColorPreviewRectList[0].color, m_ColorPreviewRectList[1].color);
        drawColorMapElementPreview();
        drawColorMapPreview();
    }
    if (m_ValueSliderLabel->rect().contains(m_ValueSliderLabel->mapFrom(this, event->pos()))) {
        int value = ColorMap::normalize(0, m_ValueSliderImage.height(),0, 255, m_ValueSliderLabel->mapFrom(this, event->pos()).y());
        m_PreviewColor.setHsv(m_PreviewColor.hue(), m_PreviewColor.saturation(), value);
        QImage colorPreviewImage = QImage(32,32,QImage::Format_RGB32);
        colorPreviewImage.fill(m_PreviewColor);
        m_ColorPreviewLabel->setPixmap(QPixmap::fromImage(colorPreviewImage));
        drawValueSlider();
        m_LightnessLineEdit->setText(QString::number(value));
    }
    int rectSize = 35;
    int verticalOffset = (m_PreviewImageHeight - rectSize)/2;
    if (m_ColorMapElementPreviewLabel->rect().contains(m_ColorMapElementPreviewLabel->mapFrom(this, event->pos()))) {
        QPoint mousePoint = m_ColorMapElementPreviewLabel->mapFrom(this, event->pos());
        for (auto& rect : m_ColorPreviewRectList) {
            int xstart = qFloor(rect.position * (m_PreviewImageWidth-rectSize));
            QRect recta(xstart,verticalOffset,rectSize,rectSize);
            if (recta.contains(mousePoint)) rect.isActive = true;
            else rect.isActive = false;
        }
        int xstart = qFloor(m_MidPreviewRect.position * (m_PreviewImageWidth-rectSize));
        QRect recta(xstart,verticalOffset,rectSize,rectSize);
        if (recta.contains(mousePoint)) m_MidPreviewRect.isActive = true;
        else m_MidPreviewRect.isActive = false;
        ColorMapDivergent *map = &dynamic_cast<ColorMapDivergent&>(*m_ColorMap);
        map->setColors(m_ColorPreviewRectList[0].color, m_ColorPreviewRectList[1].color);
        m_isMoveRect = true;
        m_MoveRectOrigin = mousePoint;
        drawColorMapElementPreview();
        drawColorMapPreview();
    }
}

void ColorMapWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (m_PaletteLabel->rect().contains(m_PaletteLabel->mapFrom(this, event->pos()))) {
        QColor mouseColor = QWidget::grab().toImage().pixelColor(event->pos());
        m_RedLineEdit->setText(QString::number(mouseColor.red()));
        m_GreenLineEdit->setText(QString::number(mouseColor.green()));
        m_BlueLineEdit->setText(QString::number(mouseColor.blue()));
        m_HueLineEdit->setText(QString::number(mouseColor.hue()));
        m_SaturationLineEdit->setText(QString::number(mouseColor.saturation()));
        m_LightnessLineEdit->setText(QString::number(mouseColor.value()));
        QImage colorPreviewImage = QImage(32,32,QImage::Format_RGB32);
        colorPreviewImage.fill(mouseColor);
        m_ColorPreviewLabel->setPixmap(QPixmap::fromImage(colorPreviewImage));
        m_PreviewColor = mouseColor;
        drawValueSlider();
        for (auto& rect : m_ColorPreviewRectList) {
            if (rect.isActive) {
                rect.color = mouseColor;
                drawColorMapElementPreview();
                drawColorMapPreview();
            }
        }
        ColorMapDivergent *map = &dynamic_cast<ColorMapDivergent&>(*m_ColorMap);
        map->setColors(m_ColorPreviewRectList[0].color, m_ColorPreviewRectList[1].color);
        drawColorMapElementPreview();
        drawColorMapPreview();
    }
    if (m_ValueSliderLabel->rect().contains(m_ValueSliderLabel->mapFrom(this, event->pos()))) {
        int value = ColorMap::normalize(0, m_ValueSliderImage.height(),0, 255, m_ValueSliderLabel->mapFrom(this, event->pos()).y());
        m_PreviewColor.setHsv(m_PreviewColor.hue(), m_PreviewColor.saturation(), value);
        QImage colorPreviewImage = QImage(32,32,QImage::Format_RGB32);
        colorPreviewImage.fill(m_PreviewColor);
        m_ColorPreviewLabel->setPixmap(QPixmap::fromImage(colorPreviewImage));
        drawValueSlider();
        for (auto& rect : m_ColorPreviewRectList) {
            if (rect.isActive) rect.color = m_PreviewColor;
        }
        drawColorMapPreview();
        drawColorMapElementPreview();
        m_LightnessLineEdit->setText(QString::number(value));

    }

    int rectSize = 35;
    int verticalOffset = (m_PreviewImageHeight - rectSize)/2;
    if (m_ColorMapElementPreviewLabel->rect().contains(m_ColorMapElementPreviewLabel->mapFrom(this, event->pos()))) {
        QPoint mousePoint = m_ColorMapElementPreviewLabel->mapFrom(this, event->pos());
        for (auto& rect : m_ColorPreviewRectList) {
            int xstart = qFloor(rect.position * (m_PreviewImageWidth-rectSize));
            QRect recta(xstart,verticalOffset,rectSize,rectSize);
            if (recta.contains(mousePoint)) {
                rect.isHover = true;
            } else rect.isHover = false;
            if (m_isMoveRect && rect.isActive) {
                int dx = mousePoint.x() - m_MoveRectOrigin.x();
                m_MoveRectOrigin = mousePoint;
                rect.position += 1.0/m_PreviewImageWidth * dx;
                if (rect.position > 1) rect.position = 1.0f;
                if (rect.position < 0) rect.position = 0.0f;
                drawColorMapPreview();
                drawColorMapElementPreview();
            }
        }
        int xstart = qFloor(m_MidPreviewRect.position * (m_PreviewImageWidth-rectSize));
        QRect recta(xstart,verticalOffset,rectSize,rectSize);
        if (recta.contains(mousePoint)) {
            m_MidPreviewRect.isHover = true;
        } else m_MidPreviewRect.isHover = false;
        if (m_isMoveRect && m_MidPreviewRect.isActive) {
            int dx = mousePoint.x() - m_MoveRectOrigin.x();
            m_MoveRectOrigin = mousePoint;
            m_MidPreviewRect.position += 1.0/m_PreviewImageWidth * dx;
            if (m_MidPreviewRect.position > 1) m_MidPreviewRect.position = 1.0f;
            if (m_MidPreviewRect.position < 0) m_MidPreviewRect.position = 0.0f;
        }
        ColorMapDivergent *map = &dynamic_cast<ColorMapDivergent&>(*m_ColorMap);
        map->setPositions(m_ColorPreviewRectList[0].position, m_ColorPreviewRectList[1].position, m_MidPreviewRect.position);
        drawColorMapPreview();
        drawColorMapElementPreview();
    }

}

void ColorMapWidget::mouseReleaseEvent(QMouseEvent *event)
{
    m_isMoveRect = false;
    if (m_MidPreviewRect.isActive) m_MidPreviewRect.isActive = false;
    drawColorMapElementPreview();
}

void ColorMapWidget::newColorMap()
{
    m_NewColorMapWidget->show();
}

void ColorMapWidget::cancelNewColorMap()
{
    m_NewColorMapWidget->hide();
}

void ColorMapWidget::appendColorMap(int type)
{
    int index = GlobalVars::g_ColorMapList.size();
    ColorMap* map;
    QStandardItem *item;
    if (type == TYPE_DIVERGENT) {
        map = new ColorMapDivergent();
        item = new QStandardItem("Divergent Map " + QString::number(index));
    } else if (type == TYPE_RAINBOW) {
        map = new ColorMapRainbow();
        item = new QStandardItem("Rainbow Map " + QString::number(index));
    } else if (type == TYPE_QUALITIVE) {
        map = new ColorMapCategorical();
        item = new QStandardItem("Categorical Map " + QString::number(index));
    } else if (type == TYPE_SEQUENTIAL) {
        map = new ColorMapSequential();
        item = new QStandardItem("Sequential Map " + QString::number(index));
    }
    GlobalVars::g_ColorMapList.append(map);
    m_NewColorMapWidget->hide();
    m_ColorMapListModel->appendRow(item);
}

void ColorMapWidget::changeColorMap(QModelIndex index)
{
    int i = index.row();
    m_ColorMap = GlobalVars::g_ColorMapList[i];
    if (m_ColorMapType = TYPE_DIVERGENT) {
        ColorMapDivergent *map = &dynamic_cast<ColorMapDivergent&>(*m_ColorMap);
        m_ColorPreviewRectList[0].color = map->getLeftColor();
        m_ColorPreviewRectList[1].color = map->getRightColor();
        m_ColorPreviewRectList[0].position = map->getLeftPosition();
        m_ColorPreviewRectList[1].position = map->getRightPosition();
        m_MidPreviewRect.position = map->getMidPosition();
        m_MidPreviewRect.color = Qt::white;
    }
    drawColorMapElementPreview();
    drawColorMapPreview();

}

void ColorMapWidget::drawHSLSquare()
{
    m_ColorPaletteImage = QImage(120,120, QImage::Format_RGB32);
    for (int x = 0; x < 120; x++) {
        for (int y = 0; y < 120; y++) {
            QColor col = QColor();
            double hue  = ColorMap::normalize(0.0, 120.0, 0.0, 1.0, double(x));
            double saturation  = ColorMap::normalize(0.0, 120.0, 0.0, 1.0, double(y));
            col.setHslF(hue, saturation, 0.5);
            m_ColorPaletteImage.setPixelColor(x,119-y,col);
        }
    }
}

void ColorMapWidget::drawHSLCircle()
{
    m_ColorPaletteImage = QImage(120,120, QImage::Format_RGB32);
    m_ColorPaletteImage.fill(QColor(255, 255, 255));
    int IMAGE_WIDTH = 120;
    int IMAGE_HEIGHT = 120;
    double IMAGE_DIAGONAL = sqrt(pow(IMAGE_WIDTH,2) + pow(IMAGE_HEIGHT,2));
    for (int x = 0; x < IMAGE_WIDTH; x++) {
        for (int y = 0; y < IMAGE_HEIGHT; y++) {
            QColor col = QColor();
            double hue  = qAtan2(qreal(y) - IMAGE_HEIGHT/2.0,qreal(x) - IMAGE_WIDTH/2.0);
            hue = ColorMap::normalize(-M_PI, M_PI, 0.0, 1.0, hue);
            double saturation = sqrt(pow(qreal(x) - IMAGE_WIDTH/2.0,2) + pow(qreal(y) - IMAGE_HEIGHT/2.0,2));
            saturation = ColorMap::normalize(0.0, double(IMAGE_WIDTH)/2.0, 0.0, 1.0, saturation);
            if (saturation > 1.0) {
                //qDebug() << saturation;
                if (saturation < 1.02) {
                    saturation = ColorMap::normalize(1.0, 1.1, 1.0, 0.0, saturation);
                    col.setHsvF(hue, saturation, 1.0);
                } else col.setHsvF(hue, 0.0, 1.0);
            } else {
                //qDebug() << hue << saturation;
                col.setHsvF(hue, saturation, 1.0);
            }

            m_ColorPaletteImage.setPixelColor(x,y,col);
        }
    }
}

void ColorMapWidget::drawValueSlider()
{
    int IMAGE_WIDTH = 20;
    int IMAGE_HEIGHT = 120;

    m_ValueSliderImage = QImage(IMAGE_WIDTH,IMAGE_HEIGHT, QImage::Format_RGB32);
    m_ValueSliderImage.fill(QColor(255, 255, 255));
    int pickedVal = ColorMap::normalize(0, 255, 0, IMAGE_HEIGHT, m_PreviewColor.value());
    for (int y = 0; y < IMAGE_HEIGHT; y++) {
        QColor color = m_PreviewColor;
        int scaledValue = ColorMap::normalize(0, IMAGE_HEIGHT-5, 0, 255, y);
        color.setHsv(color.hue(), color.saturation(), scaledValue);
        for (int x = 0; x < IMAGE_WIDTH; x++) {
            m_ValueSliderImage.setPixelColor(x, y, color);
            if (IMAGE_HEIGHT == pickedVal && (y == IMAGE_HEIGHT || y == IMAGE_HEIGHT-5)) {
                m_ValueSliderImage.setPixelColor(x,IMAGE_HEIGHT,Qt::black);
                m_ValueSliderImage.setPixelColor(x, IMAGE_HEIGHT-5, Qt::black);
            } else if (0 == pickedVal && (y == pickedVal || y == 5)) {
                m_ValueSliderImage.setPixelColor(x,0,Qt::black);
                m_ValueSliderImage.setPixelColor(x, 0+5, Qt::black);
            } else if (y == pickedVal+2 || y == pickedVal-2) {
                m_ValueSliderImage.setPixelColor(x,pickedVal+2,Qt::black);
                m_ValueSliderImage.setPixelColor(x,pickedVal-2, Qt::black);
            }
        }
    }
    m_ValueSliderLabel->setPixmap(QPixmap::fromImage(m_ValueSliderImage));
}

void ColorMapWidget::drawColorMapPreview()
{


    m_MapPreviewImage = QImage(m_PreviewImageWidth, m_PreviewImageHeight, QImage::Format_RGBX64);
    m_MapPreviewImage.fill(QColor(255, 255, 255));

//    for (int x = 0; x < m_PreviewImageWidth; x++) {
//        QColor col = m_ColorMap.getColor(double(x));
//        for (int y = 0; y < m_PreviewImageHeight; y++) {
//            m_MapPreviewImage.setPixelColor(x, y, col);
//        }
//    }

    if (m_ColorMapType = TYPE_DIVERGENT) {


        for (int x = 0; x < m_PreviewImageWidth; x++) {
            float s = ColorMap::normalize(0.0, double(m_PreviewImageWidth), 0.0, 1.0, double(x));
            QColor color = m_ColorMap->getColor(s);
            for (int y = 0; y < m_PreviewImageHeight; y++) {
                m_MapPreviewImage.setPixelColor(x, y, color);
            }
        }
        m_MapPreviewLabel->setPixmap(QPixmap::fromImage(m_MapPreviewImage));
    }







}

void ColorMapWidget::drawColorMapElementPreview()
{
    int rectSize = 35;
    int verticalOffset = (m_PreviewImageHeight - rectSize)/2;
    m_ColorMapElementPreviewImage = QImage(m_PreviewImageWidth, m_PreviewImageHeight, QImage::Format_RGBX64);
    m_ColorMapElementPreviewImage.fill(QColor(240, 240, 240));
    if (m_ColorMapType = TYPE_DIVERGENT) {
        ColorMapDivergent *map = &dynamic_cast<ColorMapDivergent&>(*m_ColorMap);
        for (auto& rect : m_ColorPreviewRectList) {
            int xstart = qFloor(rect.position * (m_PreviewImageWidth-rectSize));
            qDebug() << xstart << rect.position;
            if (rect.isHover) {
                rectSize += 10;
                verticalOffset = (m_PreviewImageHeight - rectSize)/2;
                xstart = qFloor(rect.position * (m_PreviewImageWidth-rectSize));
            }
            for (int x = 0; x < rectSize; x++) {
                for (int y = 0; y < rectSize; y++) {
                    if (x == 0 || x == 1 || x == rectSize-1 || x == rectSize-2 ||
                        y == 0 || y == 1 || y == rectSize-1 || y == rectSize-2) {
                        if (rect.isActive) m_ColorMapElementPreviewImage.setPixelColor(xstart + x, y+verticalOffset, Qt::red);
                        else m_ColorMapElementPreviewImage.setPixelColor(xstart + x, y+verticalOffset, Qt::black);
                    } else {
                        m_ColorMapElementPreviewImage.setPixelColor(xstart + x, y+verticalOffset, rect.color);
                    }
                }
            }
            rectSize = 35;
            verticalOffset = (m_PreviewImageHeight - rectSize)/2;
        }

        int xstart = qFloor(m_MidPreviewRect.position * (m_PreviewImageWidth-rectSize));
        qDebug() << xstart << m_MidPreviewRect.position;
        if (m_MidPreviewRect.isHover) {
            rectSize += 10;
            verticalOffset = (m_PreviewImageHeight - rectSize)/2;
            xstart = qFloor(m_MidPreviewRect.position * (m_PreviewImageWidth-rectSize));
        }
        for (int x = 0; x < rectSize; x++) {
            for (int y = 0; y < rectSize; y++) {
                if (x == 0 || x == 1 || x == rectSize-1 || x == rectSize-2 ||
                    y == 0 || y == 1 || y == rectSize-1 || y == rectSize-2) {
                    if (m_MidPreviewRect.isActive) m_ColorMapElementPreviewImage.setPixelColor(xstart + x, y+verticalOffset, Qt::red);
                    else m_ColorMapElementPreviewImage.setPixelColor(xstart + x, y+verticalOffset, Qt::black);
                } else {
                    m_ColorMapElementPreviewImage.setPixelColor(xstart + x, y+verticalOffset, m_MidPreviewRect.color);
                }
            }
        }
        rectSize = 35;
        verticalOffset = (m_PreviewImageHeight - rectSize)/2;

        m_ColorMapElementPreviewLabel->setPixmap(QPixmap::fromImage(m_ColorMapElementPreviewImage));
    }

}

QIcon ColorMapWidget::drawColorMapIcon(ColorMap *map)
{
    int ICON_SIZE = 25;
    QImage iconImage = QImage(ICON_SIZE, ICON_SIZE, QImage::Format_RGBX64);
    iconImage.fill(QColor(240, 240, 240));
    for (int x = 0; x < ICON_SIZE; x++) {
        QColor color;
        if (map->getType() == TYPE_DIVERGENT) {
            float s = ColorMap::normalize(0.0, double(ICON_SIZE), 0.0, 1.0, double(x));
            color = map->getColor(s);
            for (int y = 0; y < ICON_SIZE; y++) {
                iconImage.setPixelColor(x, y, color);
            }
        } else if (map->getType() == TYPE_RAINBOW) {
            float s = ColorMap::normalize(0.0, double(ICON_SIZE), 0.0, 1.0, double(x));
            color = map->getColor(s);
            for (int y = 0; y < ICON_SIZE; y++) {
                iconImage.setPixelColor(x, y, color);
            }
        } else if (map->getType() == TYPE_QUALITIVE) {
            float s = ColorMap::normalize(0.0, double(ICON_SIZE), 0.0, 1.0, double(x));
//            ColorMapCategorical *catmap = &dynamic_cast<ColorMapCategorical&>(*map);
//            if (catmap->getNumberColors() <= 4) {
//                float s = ColorMap::normalize(0.0, double(ICON_SIZE), 0.0, 1.0, double(x));
//                for (int y = 0; y < ICON_SIZE; y++) {
//                    iconImage.setPixelColor(x, y, color);
//                }
//            } else if (catmap->getNumberColors() <= 9) {

//            } else if (catmap->getNumberColors() <= 16) {

//            }
            color = map->getColor(s);
            for (int y = 0; y < ICON_SIZE; y++) {
                iconImage.setPixelColor(x, y, color);
            }
        } else if (map->getType() == TYPE_SEQUENTIAL) {
            float s = ColorMap::normalize(0.0, double(ICON_SIZE), 0.0, 1.0, double(x));
            color = map->getColor(s);
            for (int y = 0; y < ICON_SIZE; y++) {
                iconImage.setPixelColor(x, y, color);
            }

        }

    }
    return QIcon(QPixmap::fromImage(iconImage));
}
