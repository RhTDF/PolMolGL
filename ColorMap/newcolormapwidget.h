#ifndef NEWCOLORMAPWIDGET_H
#define NEWCOLORMAPWIDGET_H
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

class NewColorMapWidget  : public QWidget
{
    Q_OBJECT
public:
    NewColorMapWidget(QWidget *parent = nullptr);

signals:
    void noPressed();
    void okPressed(int mapType);

public slots:
    void okPressed();

private:
    QVBoxLayout *m_NewMapWidgetLayout;
    QComboBox *m_NewMapComboBox;
    QPushButton *m_NewMapOkBtn;
    QPushButton *m_NewMapNoBtn;
    static const int TYPE_SEQUENTIAL = 0;
    static const int TYPE_DIVERGENT = 1;
    static const int TYPE_QUALITIVE = 2;
    static const int TYPE_RAINBOW = 3;
};

#endif // NEWCOLORMAPWIDGET_H
