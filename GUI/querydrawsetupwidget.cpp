#include "querydrawsetupwidget.h"
#include "Data/globalvars.h"
#include "Data/drawquery.h"
#include "Data/vertex.h"
#include "Data/particle.h"

#include <QDebug>
#include <QDialogButtonBox>
#include <QDir>
#include <QElapsedTimer>
#include <QInputDialog>

#include <ColorMap/colormap.h>

QueryDrawSetupWidget::QueryDrawSetupWidget(QWidget *parent) : QWidget(parent)
{
    setupWidgets();
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    connect(m_QueryListView, SIGNAL(clicked(QModelIndex)),
            this, SLOT(changeSelectedQuery(QModelIndex)));
    connect(m_PointSizeSlider, SIGNAL(valueChanged(int)),
            this, SLOT(pointSizeSliderChanged(int)));
    connect(m_PathlineSizeSlider, SIGNAL(valueChanged(int)),
            this, SLOT(pathlineSizeSliderChanged(int)));
    connect(m_PointSizeSpinBox, SIGNAL(valueChanged(double)),
            this, SLOT(pointSizeSpinBoxChanged(double)));
    connect(m_PathlineSizeSpinBox, SIGNAL(valueChanged(double)),
            this, SLOT(pathlineSizeSpinBoxChanged(double)));
    connect(m_RenderContextCheckBox, SIGNAL(stateChanged(int)),
            this, SLOT(drawContextChanged(int)));
    connect(m_RenderPathCheckBox, SIGNAL(stateChanged(int)),
            this, SLOT(drawPathChanged(int)));
    connect(m_RenderPointCheckBox, SIGNAL(stateChanged(int)),
            this, SLOT(drawPointChanged(int)));
    connect(m_ChangeColorBtn, SIGNAL(released()),
            this, SLOT(changeColorPressed()));
}

void QueryDrawSetupWidget::addQuery(int query_index)
{
    QString itemText = GlobalVars::g_DrawQueryList[query_index].getQueryName();
    QStandardItem *item = new QStandardItem(itemText);
    m_QueryListModel->appendRow(item);
    m_QueryListView->setCurrentIndex(m_QueryListModel->indexFromItem(item));
    selectionTypeChanged(m_SelectionTypeComboBox->currentIndex());
}

void QueryDrawSetupWidget::changeSelectedQuery(QModelIndex index)
{
    qDebug() << "QueryDrawSetupWidget::changeSelectedQuery()";
    int i = index.row();
    qDebug() << "QueryDrawSetupWidget::changeSelectedQuery() model index: " << index.row();
    DrawQuery query = GlobalVars::g_DrawQueryList[i];
    qDebug() << "QueryDrawSetupWidget::changeSelectedQuery() query name: " << query.getQueryName();
    m_PointSizeSlider->setValue(int(query.getParticlePointSize()*100.0f));
    m_PointSizeSpinBox->setValue(query.getParticlePointSize());
    m_PathlineSizeSlider->setValue(int(query.getPathlineThickness()*100.0f));
    m_PathlineSizeSpinBox->setValue(query.getPathlineThickness());
    m_RenderContextCheckBox->setChecked(query.isContext());
    m_RenderPathCheckBox->setChecked(query.isDrawPath());
    m_RenderPointCheckBox->setChecked(query.isDrawParticle());

    int buffer_index = query.getDataBufferIndex();
    int num_classes = GlobalVars::g_AtomTypeStringList[buffer_index].size();
    QString label_text = "Number of classes: <b>";
    label_text.append(QString::number(num_classes));
    label_text.append("</b>");
    m_VarAtomNumClassesLabel->setText(label_text);
    num_classes = GlobalVars::g_MoleculeTypeStringList[buffer_index].size();
    label_text = "Number of classes: <b>";
    label_text.append(QString::number(num_classes));
    label_text.append("</b>");
    m_VarResidueNumClassesLabel->setText(label_text);

    selectionTypeChanged(m_SelectionTypeComboBox->currentIndex());


    if (query.getQueryType() == 2) emit newPolarQuery(i, query.getDataBufferIndex());

}

void QueryDrawSetupWidget::pointSizeSpinBoxChanged(double value)
{
    int query_index = m_QueryListView->currentIndex().row();
    m_PointSizeSlider->setValue(int(value*100.0));
    GlobalVars::g_DrawQueryList[query_index].setParticlePointSize(value);
    emit somethingChanged();
}

void QueryDrawSetupWidget::pathlineSizeSpinBoxChanged(double value)
{
    int query_index = m_QueryListView->currentIndex().row();
    m_PathlineSizeSlider->setValue(int(value*100.0));
    GlobalVars::g_DrawQueryList[query_index].setPathlineThickness(value);
    emit somethingChanged();
}

void QueryDrawSetupWidget::pointSizeSliderChanged(int value)
{
    int query_index = m_QueryListView->currentIndex().row();
    m_PointSizeSpinBox->setValue(value/100.0);
    GlobalVars::g_DrawQueryList[query_index].setParticlePointSize(value/100.0);
    emit somethingChanged();
}

void QueryDrawSetupWidget::pathlineSizeSliderChanged(int value)
{
    int query_index = m_QueryListView->currentIndex().row();
    m_PathlineSizeSpinBox->setValue(value/100.0);
    GlobalVars::g_DrawQueryList[query_index].setPathlineThickness(value/100.0);
    emit somethingChanged();
}

void QueryDrawSetupWidget::drawContextChanged(int state)
{
    int query_index = m_QueryListView->currentIndex().row();
    GlobalVars::g_DrawQueryList[query_index].setContext(bool(state));
    emit somethingChanged();
}

void QueryDrawSetupWidget::drawPointChanged(int state)
{
    int query_index = m_QueryListView->currentIndex().row();
    GlobalVars::g_DrawQueryList[query_index].setDrawParticle(bool(state));
    emit somethingChanged();
}

void QueryDrawSetupWidget::drawPathChanged(int state)
{
    int query_index = m_QueryListView->currentIndex().row();
    GlobalVars::g_DrawQueryList[query_index].setDrawPath(bool(state));
    emit somethingChanged();
}

void QueryDrawSetupWidget::drawNotQueryChanged(int state)
{
    int query_index = m_QueryListView->currentIndex().row();
    GlobalVars::g_DrawQueryList[query_index].setIsNotQuery(bool(state));
    emit somethingChanged();
}

void QueryDrawSetupWidget::changeColorPressed()
{
    int queryIndex = m_QueryListView->currentIndex().row();
    int buffid = GlobalVars::g_DrawQueryList[queryIndex].getDataBufferIndex();
    int num_particles = GlobalVars::g_FileHandler->getVertexList()[buffid].size();
    int num_frames = GlobalVars::g_FileHandler->getVertexList()[buffid][0].size();
    QColor col = QColorDialog::getColor();
    QVector3D colvec(col.redF(),col.greenF(),col.blueF());
    qDebug() << "Change color loop start";
    for (int i = 0; i < num_particles; i++) {
        for (int j = 0; j < num_frames; j++) {
            GlobalVars::g_FileHandler->m_VertexList[buffid][i][j].setColor(colvec);
        }
    }
    qDebug() << "Change color done";
    emit colorChanged(buffid);
}

void QueryDrawSetupWidget::newQueryPressed()
{

    QStringList queryList;
    for (int i = 0; i < GlobalVars::g_DrawQueryList.size(); i++) {
        queryList << GlobalVars::g_DrawQueryList[i].getQueryName();
    }
    QDialog  d;
    QVBoxLayout vbox;

    QHBoxLayout topLayout;
    QComboBox  comboBoxTop;
    comboBoxTop.addItems(queryList);
    QLabel labelTop;
    labelTop.setText("Base Query:");
    topLayout.addWidget(&labelTop);
    topLayout.addWidget(&comboBoxTop);

    QDialogButtonBox buttonBox;
    buttonBox.setStandardButtons(QDialogButtonBox::Ok  | QDialogButtonBox::Cancel);

    QObject::connect(&buttonBox, SIGNAL(accepted()), &d, SLOT(accept()));
    QObject::connect(&buttonBox, SIGNAL(rejected()), &d, SLOT(reject()));

    vbox.addLayout(&topLayout);
    vbox.addWidget(&buttonBox);
    d.setLayout(&vbox);

    int result = d.exec();
    if(result == QDialog::Accepted)
    {
        DrawQuery newQuery;
        DrawQuery * baseQuery = &GlobalVars::g_DrawQueryList[comboBoxTop.currentIndex()];
        qDebug() << "QueryDrawSetupWidget::newQueryPressed() baseQuery->getQueryName(): " << baseQuery->getQueryName() << "baseQuery->getDataBufferIndex(): " << baseQuery->getDataBufferIndex();
        int dataindex = baseQuery->getDataBufferIndex();
        newQuery.setBufferIndex(dataindex);
        int queryType = 2; //UNHARD CODE
        newQuery.setParentQueryIndex(dataindex);
        newQuery.setQueryType(queryType);
        newQuery.setQueryName(comboBoxTop.currentText() + " query " + QString::number(GlobalVars::g_DrawQueryList.size()));
        newQuery.setBitArray(baseQuery->getBitArray());
        GlobalVars::g_DrawQueryList.append(newQuery);
        addQuery(GlobalVars::g_DrawQueryList.size()-1);
        emit newPolarQuery(GlobalVars::g_DrawQueryList.size()-1, dataindex);
    }
}

void QueryDrawSetupWidget::velocityMapHueChanged(int state)
{
    if (state >= 1) {
        m_ColorMapListComboBox->setCurrentIndex(9);
        m_ColorMapListComboBox->setEnabled(false);
    } else {
        m_ColorMapListComboBox->setEnabled(true);
    }
}

void QueryDrawSetupWidget::colorMapVarChanged(int index)
{
    switch(index){
      case 6:
    {
        m_StackedLayout->setCurrentIndex(index);
        int query_index = m_QueryListView->currentIndex().row();
        int buffer_index = GlobalVars::g_DrawQueryList[query_index].getDataBufferIndex();
        int num_classes = GlobalVars::g_AtomTypeStringList[buffer_index].size();
        QString label_text = "Number of classes: <b>";
        label_text.append(QString::number(num_classes));
        label_text.append("</b>");
        m_VarAtomNumClassesLabel->setText(label_text);
        m_ColorMapListComboBox->setEnabled(false);
        break;
    }
    case 7:
    {
        m_StackedLayout->setCurrentIndex(index);
        int query_index = m_QueryListView->currentIndex().row();
        int buffer_index = GlobalVars::g_DrawQueryList[query_index].getDataBufferIndex();
        int num_classes = GlobalVars::g_MoleculeTypeStringList[buffer_index].size();
        QString label_text = "Number of classes: <b>";
        label_text.append(QString::number(num_classes));
        label_text.append("</b>");
        m_VarResidueNumClassesLabel->setText(label_text);
        m_ColorMapListComboBox->setEnabled(false);
        break;
    }
      default:
    {
        m_StackedLayout->setCurrentIndex(index);
        m_ColorMapListComboBox->setEnabled(true);
        break;
    }
    }


}

void QueryDrawSetupWidget::lengthColorChanged()
{
    int queryIndex = m_QueryListView->currentIndex().row();
    int buffid = GlobalVars::g_DrawQueryList[queryIndex].getDataBufferIndex();
    int num_particles = GlobalVars::g_FileHandler->getVertexList()[buffid].size();
    int num_frames = GlobalVars::g_FileHandler->getVertexList()[buffid][0].size();
    bool isLocalMaximums = m_LocalValueColorCheckBox->isChecked();
    ColorMap *color_map = GlobalVars::g_ColorMapList[m_ColorMapListComboBox->currentIndex()];
    QColor col;
    qDebug() << "QueryDrawSetupWidget:: lengthColorChanged()";
    /**** WITH Z LENGTH ****/
    if (m_VarLengthInclZCheckBox->isChecked()) {
        double max_length = 0.0;
        double min_length = 10.0;
        for (int i = 0; i < num_particles; i++) {
            if (!isLocalMaximums || GlobalVars::g_DrawQueryList[queryIndex].getBit(i)) {
                double cur_length = 0.0;
                for (int j = 0; j < num_frames-1; j++) {
                    QVector3D pos0 = GlobalVars::g_FileHandler->m_VertexList[buffid][i][j].getPosition();
                    QVector3D pos1 = GlobalVars::g_FileHandler->m_VertexList[buffid][i][j+1].getPosition();
                    double distance = pos0.distanceToPoint(pos1);
                    if (distance > 10.0) break;
                    cur_length += distance;
                }
                if (cur_length >= max_length) max_length = cur_length;
                if (cur_length <= min_length) min_length = cur_length;
            }

        }
        for (int i = 0; i < num_particles; i++) {
            double cur_length = 0.0;
            for (int j = 0; j < num_frames-1; j++) {
                QVector3D pos0 = GlobalVars::g_FileHandler->m_VertexList[buffid][i][j].getPosition();
                QVector3D pos1 = GlobalVars::g_FileHandler->m_VertexList[buffid][i][j+1].getPosition();
                double distance = pos0.distanceToPoint(pos1);
                if (distance > 10.0) break;
                cur_length += distance;
            }
            col = color_map->getColor(ColorMap::normalize(min_length, max_length, 0.0, 1.0, cur_length));
            for (int j = 0; j < num_frames; j++) {
                GlobalVars::g_FileHandler->m_VertexList[buffid][i][j].setColor(QVector3D(col.redF(), col.greenF(), col.blueF()));
            }
        }
    /**** NO Z LENGTH ****/
    } else {
        double max_length = 0.0;
        double min_length = 10.0;
        for (int i = 0; i < num_particles; i++) {
            if (!isLocalMaximums || GlobalVars::g_DrawQueryList[queryIndex].getBit(i)) {
                double cur_length = 0.0;
                for (int j = 0; j < num_frames-1; j++) {
                    QVector2D pos0 = GlobalVars::g_FileHandler->m_VertexList[buffid][i][j].getPosition().toVector2D();
                    QVector2D pos1 = GlobalVars::g_FileHandler->m_VertexList[buffid][i][j+1].getPosition().toVector2D();
                    double distance = pos0.distanceToPoint(pos1);
                    if (distance > 10.0) break;
                    cur_length += distance;
                }
                if (cur_length >= max_length) max_length = cur_length;
                if (cur_length <= min_length) min_length = cur_length;
            }
        }
        for (int i = 0; i < num_particles; i++) {
            double cur_length = 0.0;
            for (int j = 0; j < num_frames-1; j++) {
                QVector2D pos0 = GlobalVars::g_FileHandler->m_VertexList[buffid][i][j].getPosition().toVector2D();
                QVector2D pos1 = GlobalVars::g_FileHandler->m_VertexList[buffid][i][j+1].getPosition().toVector2D();
                double distance = pos0.distanceToPoint(pos1);
                if (distance > 10.0) break;
                cur_length += distance;
            }
            col = color_map->getColor(ColorMap::normalize(min_length, max_length, 0.0, 1.0, cur_length));
            for (int j = 0; j < num_frames; j++) {
                GlobalVars::g_FileHandler->m_VertexList[buffid][i][j].setColor(QVector3D(col.redF(), col.greenF(), col.blueF()));
            }
        }
    }
    emit colorChanged(buffid);
}

void QueryDrawSetupWidget::displacementColorChanged()
{
    int queryIndex = m_QueryListView->currentIndex().row();
    int buffid = GlobalVars::g_DrawQueryList[queryIndex].getDataBufferIndex();
    int num_particles = GlobalVars::g_FileHandler->getVertexList()[buffid].size();
    int num_frames = GlobalVars::g_FileHandler->getVertexList()[buffid][0].size();
    int t1 = m_VarDispT1SpinBox->value();
    int t2 = m_VarDispT2SpinBox->value();
    ColorMap *color_map = GlobalVars::g_ColorMapList[m_ColorMapListComboBox->currentIndex()];
    QColor col;
    qDebug() << "QueryDrawSetupWidget:: lengthColorChanged()";
    /**** WITH Z LENGTH ****/
    if (m_VarDispInclZCheckBox->isChecked()) {
        double max_length = 0.0;
        for (int i = 0; i < num_particles; i++) {
            QVector3D pos0 = GlobalVars::g_FileHandler->m_VertexList[buffid][i][t1].getPosition();
            QVector3D pos1 = GlobalVars::g_FileHandler->m_VertexList[buffid][i][t2].getPosition();
            double distance = pos0.distanceToPoint(pos1);
            if (distance < 10.0 && distance > max_length) max_length = distance;
        }
        for (int i = 0; i < num_particles; i++) {
            double cur_length = 0.0;
            QVector3D pos0 = GlobalVars::g_FileHandler->m_VertexList[buffid][i][t1].getPosition();
            QVector3D pos1 = GlobalVars::g_FileHandler->m_VertexList[buffid][i][t2].getPosition();
            double distance = pos0.distanceToPoint(pos1);
            if (distance < 10.0) {
                col = color_map->getColor(ColorMap::normalize(0.0, max_length, 0.0, 1.0, distance));
                for (int j = 0; j < num_frames; j++) {
                    GlobalVars::g_FileHandler->m_VertexList[buffid][i][j].setColor(QVector3D(col.redF(), col.greenF(), col.blueF()));
                }
            }
        }
    /**** NO Z LENGTH ****/
    } else {
        double max_length = 0.0;
        for (int i = 0; i < num_particles; i++) {
            QVector2D pos0 = GlobalVars::g_FileHandler->m_VertexList[buffid][i][t1].getPosition().toVector2D();
            QVector2D pos1 = GlobalVars::g_FileHandler->m_VertexList[buffid][i][t2].getPosition().toVector2D();
            double distance = pos0.distanceToPoint(pos1);
            if (distance < 10.0 && distance > max_length) max_length = distance;
        }
        for (int i = 0; i < num_particles; i++) {
            double cur_length = 0.0;
            QVector2D pos0 = GlobalVars::g_FileHandler->m_VertexList[buffid][i][t1].getPosition().toVector2D();
            QVector2D pos1 = GlobalVars::g_FileHandler->m_VertexList[buffid][i][t2].getPosition().toVector2D();
            double distance = pos0.distanceToPoint(pos1);
            if (distance < 10.0) {
                col = color_map->getColor(ColorMap::normalize(0.0, max_length, 0.0, 1.0, distance));
                for (int j = 0; j < num_frames; j++) {
                    GlobalVars::g_FileHandler->m_VertexList[buffid][i][j].setColor(QVector3D(col.redF(), col.greenF(), col.blueF()));
                }
            }
        }
    }
    emit colorChanged(buffid);

}

void QueryDrawSetupWidget::velocityColorChanged()
{
    qDebug() << "QueryDrawSetupWidget:: velocityColorChanged()";
    int queryIndex = m_QueryListView->currentIndex().row();
    int buffer_index = GlobalVars::g_DrawQueryList[queryIndex].getDataBufferIndex();
    int num_particles = GlobalVars::g_FileHandler->getVertexList()[buffer_index].size();
    int num_frames = GlobalVars::g_FileHandler->getVertexList()[buffer_index][0].size();  
    ColorMap *color_map = GlobalVars::g_ColorMapList[m_ColorMapListComboBox->currentIndex()];
    QColor color;
    double max_velocity = 0.0;
    double cur_velocity;
    if (m_VarVelocityAvgRadioBtn->isChecked()) {
        for (int i = 0; i < num_particles; i++) {
            cur_velocity = 0.0;
            for (int j = 0; j < num_frames-1; j++) {
                QVector3D pos0 = GlobalVars::g_FileHandler->m_VertexList[buffer_index][i][j].getPosition();
                QVector3D pos1 = GlobalVars::g_FileHandler->m_VertexList[buffer_index][i][j+1].getPosition();
                if (m_VarVelocityInclZCheckBox->isChecked()) {
                    double distance = pos0.distanceToPoint(pos1);
                    if (distance < 10.0) cur_velocity += distance;
                } else {
                    double distance = pos0.toVector2D().distanceToPoint(pos1.toVector2D());
                    if (distance < 10.0) cur_velocity += distance;
                }
            }
            cur_velocity = cur_velocity/(num_frames-1);
            if (cur_velocity > max_velocity) max_velocity = cur_velocity;
        }
        //max vec calculated
        for (int i = 0; i < num_particles; i++) {
            cur_velocity = 0.0;
            for (int j = 0; j < num_frames-1; j++) {
                QVector3D pos0 = GlobalVars::g_FileHandler->m_VertexList[buffer_index][i][j].getPosition();
                QVector3D pos1 = GlobalVars::g_FileHandler->m_VertexList[buffer_index][i][j+1].getPosition();
                if (m_VarVelocityInclZCheckBox->isChecked()) {
                    double distance = pos0.distanceToPoint(pos1);
                    if (distance < 10.0) cur_velocity += distance;
                } else {
                    double distance = pos0.toVector2D().distanceToPoint(pos1.toVector2D());
                    if (distance < 10.0) cur_velocity += distance;
                }
            }
            cur_velocity = cur_velocity/(num_frames-1);
            if (m_VarVelocityMapDirCheckBox->isChecked()) {
                color = QColor();
                QVector3D pos0 = GlobalVars::g_FileHandler->m_VertexList[buffer_index][i][0].getPosition();
                QVector3D pos1 = GlobalVars::g_FileHandler->m_VertexList[buffer_index][i][50].getPosition();
                double theta = atan2(pos1.y() - pos0.y(), pos1.x() - pos0.x());
                double hue = ColorMap::normalize(-M_PI, M_PI, 0.0, 1.0, theta);
                double value = ColorMap::normalize(0.0, max_velocity, 0.1, 0.7, cur_velocity);
                color.setHsvF(hue, 1.0, value);
                for (int j = 0; j < num_frames-1; j++) {
                    GlobalVars::g_FileHandler->m_VertexList[buffer_index][i][j].setColor(
                                QVector3D(color.redF(), color.greenF(), color.blueF()));
                }
            } else {
                color = color_map->getColor(ColorMap::normalize(0.0, max_velocity, 0.0, 1.0, cur_velocity));
                for (int j = 0; j < num_frames-1; j++) {
                    GlobalVars::g_FileHandler->m_VertexList[buffer_index][i][j].setColor(
                                QVector3D(color.redF(), color.greenF(), color.blueF()));
                }
            }
        }

    } else {

    }
    QWidget *widget = new QWidget;

    int ICON_SIZE = 500;
    QImage iconImage = QImage(ICON_SIZE, ICON_SIZE, QImage::Format_RGBX64);
    iconImage.fill(QColor(240, 240, 240));
    QVector2D orig(249,249);
    QColor col;
    col = QColor();
    double maxdist = 250;
    for (int x = 0; x < 500; x++) {
        for (int y = 0; y < 500; y++) {
            double theta = atan2(249 - y, 249 - x);
            double hue = ColorMap::normalize(-M_PI, M_PI, 0.0, 1.0, theta);

            QVector2D pt(x,y);
            double distance = orig.distanceToPoint(pt);
            double value = ColorMap::normalize(0.0, maxdist, 0.1, 0.7, distance);
            if (distance > 250) col.setRgbF(1.0,1.0,1.0);
            else col.setHsvF(hue, 1, value);
            iconImage.setPixelColor(x, y, col);
        }
    }
    QLabel *myLabel = new QLabel;
        myLabel->setPixmap(QPixmap::fromImage(iconImage));
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(myLabel);
    widget->setLayout(layout);
    widget->show();

    QImage colomapimage = QImage(1000, 100, QImage::Format_RGBX64);
    ColorMap *colormap = GlobalVars::g_ColorMapList[0];
    for (int x = 0; x < 1000; x++) {
        double value = ColorMap::normalize(0.0, 1000.0, 0.0, 1.0, double(x));
        QColor col = colormap->getColor(value);
        for (int y = 0; y < 100; y++) {
            colomapimage.setPixelColor(x,y, col);
        }
    }
    colomapimage.save("colormap1.jpg", "jpg", 100);

    colormap = GlobalVars::g_ColorMapList[7];
    for (int x = 0; x < 1000; x++) {
        double value = ColorMap::normalize(0.0, 1000.0, 0.0, 1.0, double(x));
        QColor col = colormap->getColor(value);
        for (int y = 0; y < 100; y++) {
            colomapimage.setPixelColor(x,y, col);
        }
    }
    colomapimage.save("colormap2.jpg", "jpg", 100);

    colormap = GlobalVars::g_ColorMapList[12];
    for (int x = 0; x < 1000; x++) {
        double value = ColorMap::normalize(0.0, 1000.0, 0.0, 1.0, double(x));
        QColor col = colormap->getColor(value);
        for (int y = 0; y < 100; y++) {
            colomapimage.setPixelColor(x,y, col);
        }
    }
    colomapimage.save("colormap3.jpg", "jpg", 100);

    colormap = GlobalVars::g_ColorMapList[14];
    for (int x = 0; x < 1000; x++) {
        double value = ColorMap::normalize(0.0, 1000.0, 0.0, 1.0, double(x));
        QColor col = colormap->getColor(value);
        for (int y = 0; y < 100; y++) {
            colomapimage.setPixelColor(x,y, col);
        }
    }
    colomapimage.save("colormap4.jpg", "jpg", 100);

    emit colorChanged(buffer_index);
}

void QueryDrawSetupWidget::depthColorChanged()
{
        qDebug() << "QueryDrawSetupWidget:: depthColorChanged()";
    int queryIndex = m_QueryListView->currentIndex().row();
    int buffid = GlobalVars::g_DrawQueryList[queryIndex].getDataBufferIndex();
    int num_particles = GlobalVars::g_FileHandler->getVertexList()[buffid].size();
    int num_frames = GlobalVars::g_FileHandler->getVertexList()[buffid][0].size();
    ColorMap *color_map = GlobalVars::g_ColorMapList[m_ColorMapListComboBox->currentIndex()];
    QColor color;
    double max_depth = -100.0;
    double min_depth = 100.0;
    double cur_depth;

    if (m_VarDepthChangeCheckBox->isChecked()) {
        for (int i = 0; i < num_particles; i++) {
            double start_depth = GlobalVars::g_FileHandler->m_VertexList[buffid][i].front().getPosition().z();
            double end_depth = GlobalVars::g_FileHandler->m_VertexList[buffid][i].back().getPosition().z();
            double depth_change = start_depth - end_depth;

            if (depth_change >= max_depth) max_depth = depth_change;
            if (depth_change <= min_depth) min_depth = depth_change;
        }
        for (int i = 0; i < num_particles; i++) {
            double start_depth = GlobalVars::g_FileHandler->m_VertexList[buffid][i].front().getPosition().z();
            double end_depth = GlobalVars::g_FileHandler->m_VertexList[buffid][i].back().getPosition().z();
            double depth_change = start_depth - end_depth;
            if (depth_change < 0) color = color_map->getColor(ColorMap::normalize(min_depth, 0.0, 0.0, 0.5, depth_change));
            else color = color_map->getColor(ColorMap::normalize(0.0, max_depth, 0.5, 1.0, depth_change));

            for (int j = 0; j < num_frames; j++) {
                GlobalVars::g_FileHandler->m_VertexList[buffid][i][j].setColor(QVector3D(color.redF(), color.greenF(), color.blueF()));
            }
        }

    } else {
        for (int i = 0; i < num_particles; i++) {
            for (int j = 0; j < num_frames; j++) {
                cur_depth = GlobalVars::g_FileHandler->m_VertexList[buffid][i][j].getPosition().z();
                if (cur_depth >= max_depth) max_depth = cur_depth;
                if (cur_depth <= min_depth) min_depth = cur_depth;
            }
        }

        if (m_VarDepthStartCheckBox->isChecked()) {
            for (int i = 0; i < num_particles; i++) {
                cur_depth = GlobalVars::g_FileHandler->m_VertexList[buffid][i].front().getPosition().z();
                color = color_map->getColor(ColorMap::normalize(min_depth, max_depth, 0.0, 1.0, cur_depth));
                for (int j = 0; j < num_frames; j++) {
                    GlobalVars::g_FileHandler->m_VertexList[buffid][i][j].setColor(QVector3D(color.redF(), color.greenF(), color.blueF()));
                }
            }
        } else if (m_VarDepthEndCheckBox->isChecked()) {
            for (int i = 0; i < num_particles; i++) {
                cur_depth = GlobalVars::g_FileHandler->m_VertexList[buffid][i].back().getPosition().z();
                color = color_map->getColor(ColorMap::normalize(min_depth, max_depth, 0.0, 1.0, cur_depth));
                for (int j = 0; j < num_frames; j++) {
                    GlobalVars::g_FileHandler->m_VertexList[buffid][i][j].setColor(QVector3D(color.redF(), color.greenF(), color.blueF()));
                }
            }
        } else if (m_VarDepthCurCheckBox->isChecked()) {
            for (int i = 0; i < num_particles; i++) {
                for (int j = 0; j < num_frames; j++) {
                    cur_depth = GlobalVars::g_FileHandler->m_VertexList[buffid][i][j].getPosition().z();
                    color = color_map->getColor(ColorMap::normalize(min_depth, max_depth, 0.0, 1.0, cur_depth));
                    GlobalVars::g_FileHandler->m_VertexList[buffid][i][j].setColor(QVector3D(color.redF(), color.greenF(), color.blueF()));
                }
            }
        }
    }

    emit colorChanged(buffid);

    QCheckBox *m_VarDepthCurCheckBox;
    QCheckBox *m_VarDepthStartCheckBox;
    QCheckBox *m_VarDepthEndCheckBox;
    QCheckBox *m_VarDepthChangeCheckBox;


}

void QueryDrawSetupWidget::curvatureColorChanged()
{
    qDebug() << "QueryDrawSetupWidget:: curvatureColorChanged()";
}

void QueryDrawSetupWidget::bearingColorChanged()
{
    qDebug() << "QueryDrawSetupWidget:: bearingColorChanged()";
}

void QueryDrawSetupWidget::atomColorChanged()
{
    qDebug() << "QueryDrawSetupWidget:: atomColorChanged()";
    int queryIndex = m_QueryListView->currentIndex().row();
    int buffer_index = GlobalVars::g_DrawQueryList[queryIndex].getDataBufferIndex();
    int num_particles = GlobalVars::g_FileHandler->getVertexList()[buffer_index].size();
    int num_frames = GlobalVars::g_FileHandler->getVertexList()[buffer_index][0].size();
    int num_classes = GlobalVars::g_AtomTypeStringList[buffer_index].size();
    qDebug() << m_VarAtomSingleHueCheckBox->isChecked();
    if (m_VarAtomSingleHueCheckBox->isChecked()) {
        ColorMap *color_map = GlobalVars::g_ColorMapList[13];
        QColor color;
        for (int i = 0; i < num_particles; i++) {
            double value = ((double)(i%num_classes))/(double)num_classes;

            color = color_map->getColor(value);
            for (int j = 0; j < num_frames; j++) {
                GlobalVars::g_FileHandler->m_VertexList[buffer_index][i][j].setColor(QVector3D(color.redF(), color.greenF(), color.blueF()));
            }
        }
    } else {
        ColorMap *color_map = GlobalVars::g_ColorMapList[8];
        QColor color;
        for (int i = 0; i < num_particles; i++) {
            double value = ((double)(i%num_classes))/num_classes;
            color = color_map->getColor(value);
            for (int j = 0; j < num_frames; j++) {
                GlobalVars::g_FileHandler->m_VertexList[buffer_index][i][j].setColor(QVector3D(color.redF(), color.greenF(), color.blueF()));
            }
        }
    }
    emit colorChanged(buffer_index);
}

void QueryDrawSetupWidget::residueColorChanged()
{
    qDebug() << "QueryDrawSetupWidget:: residueColorChanged()";
    int queryIndex = m_QueryListView->currentIndex().row();
    int buffer_index = GlobalVars::g_DrawQueryList[queryIndex].getDataBufferIndex();
    int num_particles = GlobalVars::g_FileHandler->getVertexList()[buffer_index].size();
    int num_frames = GlobalVars::g_FileHandler->getVertexList()[buffer_index][0].size();
    int num_classes = GlobalVars::g_MoleculeTypeStringList[buffer_index].size();
    qDebug() << m_VarResidueSingleHueCheckBox->isChecked();
    if (m_VarResidueSingleHueCheckBox->isChecked()) {
        ColorMap *color_map = GlobalVars::g_ColorMapList[13];
        QColor color;
        for (int i = 0; i < num_particles; i++) {
            QString residue_name = GlobalVars::g_FileHandler->m_ParticleList[buffer_index][i].getResName();
            int residue_index = GlobalVars::g_MoleculeTypeStringList[buffer_index].indexOf(residue_name);
            double value = ((double)(residue_index%num_classes))/(double)num_classes;
            color = color_map->getColor(value);
            for (int j = 0; j < num_frames; j++) {
                GlobalVars::g_FileHandler->m_VertexList[buffer_index][i][j].setColor(QVector3D(color.redF(), color.greenF(), color.blueF()));
            }
        }
    } else {
        ColorMap *color_map;
        double value;
        if (num_classes <= 4) color_map = GlobalVars::g_ColorMapList[6];
        else if (num_classes <= 8) color_map = GlobalVars::g_ColorMapList[7];
        else color_map = GlobalVars::g_ColorMapList[8];


        QColor color;
        for (int i = 0; i < num_particles; i++) {
            Particle particle = GlobalVars::g_FileHandler->m_ParticleList[buffer_index][i];
            int residue_index = GlobalVars::g_MoleculeTypeStringList[buffer_index].indexOf(particle.getResName());

            if (num_classes > 8) {
                value = ((double)(residue_index%num_classes))/num_classes;
                color = color_map->getColor(value);
            } else color = color_map->getColor(residue_index);

            for (int j = 0; j < num_frames; j++) {
                GlobalVars::g_FileHandler->m_VertexList[buffer_index][i][j].setColor(QVector3D(color.redF(), color.greenF(), color.blueF()));
            }
        }
    }
    emit colorChanged(buffer_index);
}

void QueryDrawSetupWidget::moleculeColorChanged()
{
    qDebug() << "QueryDrawSetupWidget:: moleculeColorChanged()";
}

void QueryDrawSetupWidget::clusterColorChanged()
{
    qDebug() << "QueryDrawSetupWidget:: clusterColorChanged()";
}

void QueryDrawSetupWidget::selectionTypeChanged(int index)
{
    int query_index = m_QueryListView->currentIndex().row();
    int buffer_index = GlobalVars::g_DrawQueryList[query_index].getDataBufferIndex();
    int selection_type = index; // 0 = atom, 1 = residue

    switch(selection_type) {
        case 0: {
            m_SelectionOutModel->clear();
            m_SelectionInModel->clear();
            int nRows = GlobalVars::g_AtomTypeStringList[buffer_index].size();
            for (int x = 0; x < nRows; x++) {
                QString text = GlobalVars::g_AtomTypeStringList[buffer_index][x];
                QStandardItem *item = new QStandardItem(text);
                m_SelectionOutModel->appendRow(item);
            }
            break;
        }
        case 1: {
            m_SelectionOutModel->clear();
            m_SelectionInModel->clear();
            int nRows = GlobalVars::g_MoleculeTypeStringList[buffer_index].size();
            for (int x = 0; x < nRows; x++) {
                QString text = GlobalVars::g_MoleculeTypeStringList[buffer_index][x];
                QStandardItem *item = new QStandardItem(text);
                qDebug() << "QueryDrawSetupWidget:: selectionTypeChanged() text: " << text;
                m_SelectionOutModel->appendRow(item);
            }
            break;
        }
        default: {
            break;
        }
    }
}
/* '<' */
void QueryDrawSetupWidget::selectionLeftButtonPressed()
{
    if (m_SelectionInModel->rowCount() < 1) return;
    if (!m_SelectionInListView->currentIndex().isValid()) return;
    int query_index = m_QueryListView->currentIndex().row();
    int buffer_index = GlobalVars::g_DrawQueryList[query_index].getDataBufferIndex();
    int selection_index = m_SelectionInListView->currentIndex().row();
    QList<QStandardItem*> item = m_SelectionInModel->takeRow(selection_index);
    m_SelectionOutModel->appendRow(item[0]);
}
/* '>' */
void QueryDrawSetupWidget::selectionRightButtonPressed()
{
    if (m_SelectionOutModel->rowCount() < 1) return;
    if (!m_SelectionOutListView->currentIndex().isValid()) return;
    int query_index = m_QueryListView->currentIndex().row();
    int buffer_index = GlobalVars::g_DrawQueryList[query_index].getDataBufferIndex();
    int selection_index = m_SelectionOutListView->currentIndex().row();
    QList<QStandardItem*> item = m_SelectionOutModel->takeRow(selection_index);
    m_SelectionInModel->appendRow(item[0]);
}

void QueryDrawSetupWidget::selectionApplyButtonPressed()
{
    int query_index = m_QueryListView->currentIndex().row();
    int buffer_index = GlobalVars::g_DrawQueryList[query_index].getDataBufferIndex();
    int selection_type = m_SelectionTypeComboBox->currentIndex();
    int num_particles = GlobalVars::g_FileHandler->getVertexList()[buffer_index].size();
    DrawQuery *query = &GlobalVars::g_DrawQueryList[query_index];
    DrawQuery *parentQuery = &GlobalVars::g_DrawQueryList[buffer_index];
    switch(selection_type) {
    case 0: {
        int nRows = m_SelectionInModel->rowCount();
        for (int i = 0; i < num_particles; i++) {
            if (parentQuery->getBit(i)) {
                QString particle_atom_name = GlobalVars::g_FileHandler->m_ParticleList[buffer_index][i].getAtomName();
                bool found = false;
                for (int x = 0; x < nRows; x++) {
                    QString allowed_atom_name = m_SelectionInModel->item(x)->text();
                    if (particle_atom_name.compare(allowed_atom_name, Qt::CaseInsensitive) == 0) found = true;
                }
                query->setAttribBit(i, found);
            }
        }
        break;
    } case 1: {
        int nRows = m_SelectionInModel->rowCount();
        for (int i = 0; i < num_particles; i++) {
            if (parentQuery->getBit(i)) {
                QString particle_residue_name = GlobalVars::g_FileHandler->m_ParticleList[buffer_index][i].getResName();
                bool found = false;
                for (int x = 0; x < nRows; x++) {
                    QString allowed_residue_name = m_SelectionInModel->item(x)->text();
                    if (particle_residue_name.compare(allowed_residue_name, Qt::CaseInsensitive) == 0) found = true;
                }
                query->setAttribBit(i, found);
            }
        }
        break;
    } default: {
        break;
    }
    }    
    emit queryChanged();
    emit somethingChanged();
}

void QueryDrawSetupWidget::filterApplyButtonPressed()
{
    int query_index = m_QueryListView->currentIndex().row();
    int buffer_index = GlobalVars::g_DrawQueryList[query_index].getDataBufferIndex();
    int num_particles = GlobalVars::g_FileHandler->getVertexList()[buffer_index].size();
    DrawQuery *query = &GlobalVars::g_DrawQueryList[query_index];

    float xmax = m_FilterXMaxSpinBox->value();
    float xmin = m_FilterXMinSpinBox->value();
    float ymax = m_FilterYMaxSpinBox->value();
    float ymin = m_FilterYMinSpinBox->value();
    float zmax = m_FilterZMaxSpinBox->value();
    float zmin = m_FilterZMinSpinBox->value();

    for (int i = 0; i < num_particles; i++) {
        QVector3D pos = GlobalVars::g_FileHandler->m_VertexList[buffer_index][i][0].getPosition();
        query->setAuxBit(i, (pos.x() < xmax && pos.x() > xmin && pos.y() < ymax && pos.y() > ymin && pos.z() < zmax && pos.z() > zmin));
    }
    emit queryChanged();
    emit somethingChanged();

}

void QueryDrawSetupWidget::setupWidgets()
{
    m_MainLayout = new QHBoxLayout;
    m_TabWidget = new QTabWidget;

    setupQueryList();
    m_MainLayout->addWidget(m_TabWidget);
    setupRenderingStyleTab();
    setupColoringTab();
    setupSelectionTab();
    setupFilteringTab();

    setLayout(m_MainLayout);

}

void QueryDrawSetupWidget::setupQueryList()
{
    m_QueryListView = new QListView;
    m_QueryListModel = new QStandardItemModel;
    m_QueryListView->setModel(m_QueryListModel);
    m_QueryListView->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    m_QueryListView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    m_NewQueryBtn = new QPushButton("New Query");
    connect(m_NewQueryBtn, SIGNAL(clicked()), this, SLOT(newQueryPressed()));

    QVBoxLayout *queryListLayout = new QVBoxLayout;
    queryListLayout->addWidget(m_QueryListView);
    queryListLayout->addWidget(m_NewQueryBtn);
    m_MainLayout->addLayout(queryListLayout);

}

void QueryDrawSetupWidget::setupRenderingStyleTab()
{
    int SLIDER_MIN_SIZE = 0;
    int SLIDER_MAX_SIZE = 1000;
    int SLIDER_INTERVAL = 1;
    int SLIDER_INITIAL_VALUE = 100;
    double SPINBOX_MIN_SIZE = 0.0;
    double SPINBOX_MAX_SIZE = 10.0;
    double SPINBOX_INTERVAL = 0.1;
    double SPINBOX_INITIAL_VALUE = 1.0;

    QHBoxLayout *pointDrawSizeLayout = new QHBoxLayout;
    m_PointSizeSlider = new QSlider(Qt::Horizontal);
    m_PointSizeSlider->setRange(SLIDER_MIN_SIZE, SLIDER_MAX_SIZE);
    m_PointSizeSlider->setTickInterval(SLIDER_INTERVAL);
    m_PointSizeSlider->setValue(SLIDER_INITIAL_VALUE);
    m_PointSizeSpinBox = new QDoubleSpinBox;
    m_PointSizeSpinBox->setDecimals(3);
    m_PointSizeSpinBox->setRange(SPINBOX_MIN_SIZE, SPINBOX_MAX_SIZE);
    m_PointSizeSpinBox->setSingleStep(SPINBOX_INTERVAL);
    m_PointSizeSpinBox->setValue(SPINBOX_INITIAL_VALUE);
    m_RenderPointCheckBox = new QCheckBox("Draw Point:");
    pointDrawSizeLayout->addWidget(m_RenderPointCheckBox);
    pointDrawSizeLayout->addWidget(m_PointSizeSlider);
    pointDrawSizeLayout->addWidget(m_PointSizeSpinBox);

    QHBoxLayout *pathDrawSizeLayout = new QHBoxLayout;
    m_PathlineSizeSlider = new QSlider(Qt::Horizontal);
    m_PathlineSizeSlider->setRange(SLIDER_MIN_SIZE, SLIDER_MAX_SIZE);
    m_PathlineSizeSlider->setTickInterval(SLIDER_INTERVAL);
    m_PathlineSizeSlider->setValue(SLIDER_INITIAL_VALUE);
    m_PathlineSizeSpinBox = new QDoubleSpinBox ;
    m_PathlineSizeSpinBox->setDecimals(3);
    m_PathlineSizeSpinBox->setRange(SPINBOX_MIN_SIZE, SPINBOX_MAX_SIZE);
    m_PathlineSizeSpinBox->setSingleStep(SPINBOX_INTERVAL);
    m_PathlineSizeSpinBox->setValue(SPINBOX_INITIAL_VALUE);
    m_RenderPathCheckBox = new QCheckBox("Draw Path:");
    pathDrawSizeLayout->addWidget(m_RenderPathCheckBox);
    pathDrawSizeLayout->addWidget(m_PathlineSizeSlider);
    pathDrawSizeLayout->addWidget(m_PathlineSizeSpinBox);

    m_NotQueryCheckBox = new QCheckBox("Query Negation (Logical Not)");
    connect(m_NotQueryCheckBox, SIGNAL(stateChanged(int)), this, SLOT(drawNotQueryChanged(int)));

    QWidget *renderStyleWidget = new QWidget;
    QVBoxLayout *renderStyleWidgetLayout = new QVBoxLayout;
    renderStyleWidgetLayout->addLayout(pointDrawSizeLayout);
    renderStyleWidgetLayout->addLayout(pathDrawSizeLayout);
    renderStyleWidgetLayout->addWidget(m_NotQueryCheckBox);
    renderStyleWidget->setLayout(renderStyleWidgetLayout);
    m_TabWidget->addTab(renderStyleWidget, "Render Style");

}

void QueryDrawSetupWidget::setupColoringTab()
{
    qDebug() << "QueryDrawSetupWidget:: setupColoringTab()";

    QWidget *colorWidget = new QWidget;
    m_ColorAreaLayout = new QHBoxLayout;

    QVBoxLayout *comboBoxAreaLayout = new QVBoxLayout;

    m_ColorMapListComboBox = new QComboBox;
    m_ColorMapVarComboBox = new QComboBox;
    QStringList colorMapList;
    QStringList varList;
    int div_map_count = 0;
    int cat_map_count = 0;
    int rainbow_map_count = 0;
    int seq_map_count = 0;
    QString map_name;
    for (int i = 0; i < GlobalVars::g_ColorMapList.size(); i++) {
        ColorMap *map = GlobalVars::g_ColorMapList[i];

        switch(map->getType()){
              case 0: {
                seq_map_count++;
                map_name = "Sequential Map ";
                map_name.append(QString::number(seq_map_count));
                break;
            }  case 1: {
                div_map_count++;
                map_name = "Divergent Map ";
                map_name.append(QString::number(div_map_count));
                break;
            }  case 2: {
                cat_map_count++;
                map_name = "Categorical Map ";
                map_name.append(QString::number(cat_map_count));
                break;
            }  case 3: {
                rainbow_map_count++;
                map_name = "Rainbow Map ";
                map_name.append(QString::number(rainbow_map_count));
                break;
            } default: {
                map_name = "Unknown Map";
                break;
            }
        }
        QIcon icon = drawColorMapIcon(map);
        m_ColorMapListComboBox->addItem(icon, map_name);
    }

    varList << "Length" << "Displacement" << "Velocity" << "Depth" << "Curvature" <<
               "Bearing" << "Atom" << "Residue" << "Molecule" << "Cluster";
    //m_ColorMapListComboBox->addItems(colorMapList);
    m_ColorMapVarComboBox->addItems(varList);
    QLabel *colorMapListLabel = new QLabel("Color Map:");
    QLabel *colorMapVarLabel = new QLabel("Mapping Variable:");

    connect(m_ColorMapVarComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(colorMapVarChanged(int)));

    comboBoxAreaLayout->addWidget(colorMapVarLabel,0,Qt::AlignTop);
    comboBoxAreaLayout->addWidget(m_ColorMapVarComboBox,0,Qt::AlignTop);
    comboBoxAreaLayout->addWidget(colorMapListLabel,0,Qt::AlignTop);
    comboBoxAreaLayout->addWidget(m_ColorMapListComboBox,0,Qt::AlignTop);
    comboBoxAreaLayout->setContentsMargins(-1, -1, -1, 0);
    comboBoxAreaLayout->setSpacing(0);
    comboBoxAreaLayout->setStretch(-1, 5);

    m_ColorAreaLayout->addLayout(comboBoxAreaLayout);
    colorWidget->setLayout(m_ColorAreaLayout);
    m_TabWidget->addTab(colorWidget, "Color");

    QVBoxLayout *optionsLayout = new QVBoxLayout;
    m_RenderContextCheckBox = new QCheckBox("Render as Context:");
    m_LocalValueColorCheckBox = new QCheckBox("Color using local maximums:");
    m_ChangeColorBtn = new QPushButton("Pick Color");
    optionsLayout->addWidget(m_RenderContextCheckBox);
    optionsLayout->addWidget(m_LocalValueColorCheckBox);
    optionsLayout->addWidget(m_ChangeColorBtn);
    comboBoxAreaLayout->addLayout(optionsLayout);

    setupLengthColorArea();
    setupDisplacementColorArea();
    setupVelocityColorArea();
    setupDepthColorArea();
    setupCurvatureColorArea();
    setupBearingColorArea();
    setupAtomColorArea();
    setupResidueColorArea();
    setupMoleculeColorArea();
    setupClusterColorArea();

    qDebug() << "QueryDrawSetupWidget:: setupColoringTab()";
    m_StackedLayout = new QStackedLayout;
    m_StackedLayout->addWidget(m_VarLengthColorWidget);
    m_StackedLayout->addWidget(m_VarDispColorWidget);
    m_StackedLayout->addWidget(m_VarVelocityColorWidget);
    m_StackedLayout->addWidget(m_VarDepthColorWidget);
    m_StackedLayout->addWidget(m_VarCurvatureColorWidget);
    m_StackedLayout->addWidget(m_VarBearingColorWidget);
    m_StackedLayout->addWidget(m_VarAtomColorWidget);
    m_StackedLayout->addWidget(m_VarResidueColorWidget);
    m_StackedLayout->addWidget(m_VarMoleculeColorWidget);
    m_StackedLayout->addWidget(m_VarClusterColorWidget);

    m_ColorAreaLayout->addLayout(m_StackedLayout);

}

void QueryDrawSetupWidget::setupSelectionTab()
{
    QWidget *selectionWidget = new QWidget();   
    QHBoxLayout *mainLayout = new QHBoxLayout;
    selectionWidget->setLayout(mainLayout);

    QVBoxLayout *firstVBoxLayout = new QVBoxLayout;
    m_SelectionTypeComboBox = new QComboBox;
    m_SelectionTypeComboBox->addItem("Atom Name");
    m_SelectionTypeComboBox->addItem("Resisdue Name");
    //m_SelectionTypeComboBox->addItem("Molecule Name");
    m_SelectionApplyButton = new QPushButton("Apply");
    firstVBoxLayout->addWidget(m_SelectionTypeComboBox);
    firstVBoxLayout->addSpacerItem(new QSpacerItem(1,1));
    firstVBoxLayout->addWidget(m_SelectionApplyButton);

    QFont font;
    font.setPointSize(8);
    QVBoxLayout *secondVBoxLayout = new QVBoxLayout;
    m_SelectionRightButton = new QPushButton(">");
    m_SelectionRightButton->setFixedSize(25,25);
    m_SelectionRightButton->setFont(font);
    m_SelectionLeftButton = new QPushButton("<");
    m_SelectionLeftButton->setMaximumSize(25,25);
    m_SelectionLeftButton->setFont(font);
    secondVBoxLayout->addWidget(m_SelectionRightButton);
    secondVBoxLayout->addWidget(m_SelectionLeftButton);

    m_SelectionOutListView = new QListView;
    m_SelectionOutModel = new QStandardItemModel;
    m_SelectionOutListView->setModel(m_SelectionOutModel);
    m_SelectionOutListView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    m_SelectionInListView = new QListView;
    m_SelectionInModel = new QStandardItemModel;
    m_SelectionInListView->setModel(m_SelectionInModel);
    m_SelectionInListView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    mainLayout->addLayout(firstVBoxLayout);
    mainLayout->addWidget(m_SelectionOutListView);
    mainLayout->addLayout(secondVBoxLayout);
    mainLayout->addWidget(m_SelectionInListView);
    mainLayout->addStretch();

    m_TabWidget->addTab(selectionWidget, "Selection");

    connect(m_SelectionTypeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(selectionTypeChanged(int)));
    connect(m_SelectionRightButton, SIGNAL(released()), this, SLOT(selectionRightButtonPressed()));
    connect(m_SelectionLeftButton, SIGNAL(released()), this, SLOT(selectionLeftButtonPressed()));
    connect(m_SelectionApplyButton, SIGNAL(released()), this, SLOT(selectionApplyButtonPressed()));

    m_SelectionTypeComboBox->setCurrentIndex(0);
}

void QueryDrawSetupWidget::setupFilteringTab()
{
    double X_MAX = 117.0;
    double Y_MAX = 117.0;
    double Z_MAX = 10.0;
    double X_STEP = 1.0;
    double Y_STEP = 1.0;
    double Z_STEP = 0.5;

    QWidget *filteringWidget = new QWidget;
    QHBoxLayout *mainLayout = new QHBoxLayout;
    QVBoxLayout *leftLayout = new QVBoxLayout;
    QVBoxLayout *rightLayout = new QVBoxLayout;
    filteringWidget->setLayout(mainLayout);
    mainLayout->addLayout(leftLayout);
    mainLayout->addSpacerItem(new QSpacerItem(0,0));
    mainLayout->addLayout(rightLayout);

    m_FilterXMinSpinBox = new QDoubleSpinBox;
    m_FilterXMinSpinBox->setMaximum(X_MAX);
    m_FilterXMinSpinBox->setMinimum(0);
    m_FilterXMinSpinBox->setSingleStep(X_STEP);
        m_FilterXMaxSpinBox = new QDoubleSpinBox;
    m_FilterXMaxSpinBox->setMaximum(X_MAX);
    m_FilterXMaxSpinBox->setMinimum(0);
    m_FilterXMaxSpinBox->setSingleStep(X_STEP);
        m_FilterYMinSpinBox = new QDoubleSpinBox;
    m_FilterYMinSpinBox->setMaximum(Y_MAX);
    m_FilterYMinSpinBox->setMinimum(0);
    m_FilterYMinSpinBox->setSingleStep(Y_STEP);
        m_FilterYMaxSpinBox = new QDoubleSpinBox;
    m_FilterYMaxSpinBox->setMaximum(Y_MAX);
    m_FilterYMaxSpinBox->setMinimum(0);
    m_FilterYMaxSpinBox->setSingleStep(Y_STEP);
        m_FilterZMinSpinBox = new QDoubleSpinBox;
    m_FilterZMinSpinBox->setMaximum(Z_MAX);
    m_FilterZMinSpinBox->setMinimum(0);
    m_FilterZMinSpinBox->setSingleStep(Z_STEP);
        m_FilterZMaxSpinBox = new QDoubleSpinBox;
    m_FilterZMaxSpinBox->setMaximum(Z_MAX);
    m_FilterZMaxSpinBox->setMinimum(0);
    m_FilterZMaxSpinBox->setSingleStep(Z_STEP);

    QHBoxLayout *topLeftLayout = new QHBoxLayout;
    QHBoxLayout *midLeftLayout = new QHBoxLayout;
    QHBoxLayout *botLeftLayout = new QHBoxLayout;
    leftLayout->addLayout(topLeftLayout);
    leftLayout->addLayout(midLeftLayout);
    leftLayout->addLayout(botLeftLayout);
    m_FilterApplyButton = new QPushButton("Apply");
    leftLayout->addWidget(m_FilterApplyButton);

    topLeftLayout->addWidget(new QLabel("Min x:"));
    topLeftLayout->addWidget(m_FilterXMinSpinBox);
    topLeftLayout->addWidget(new QLabel("Max x:"));
    topLeftLayout->addWidget(m_FilterXMaxSpinBox);
    midLeftLayout->addWidget(new QLabel("Min y:"));
    midLeftLayout->addWidget(m_FilterYMinSpinBox);
    midLeftLayout->addWidget(new QLabel("Max y:"));
    midLeftLayout->addWidget(m_FilterYMaxSpinBox);
    botLeftLayout->addWidget(new QLabel("Min z:"));
    botLeftLayout->addWidget(m_FilterZMinSpinBox);
    botLeftLayout->addWidget(new QLabel("Max z:"));
    botLeftLayout->addWidget(m_FilterZMaxSpinBox);

    connect(m_FilterApplyButton, SIGNAL(released()), this, SLOT(filterApplyButtonPressed()));

    int LENGTH_MAX = 1000;
    int DISPLACEMENT_MAX = 1000;
    int DEPTH_MAX = 1000;


    m_FilterLengthSlider = new RangeSlider(Qt::Horizontal);
    m_FilterDispSlider = new RangeSlider(Qt::Horizontal);
    m_FilterDepthSlider = new RangeSlider(Qt::Horizontal);

    m_FilterLengthSlider->setMaximum(LENGTH_MAX);
    m_FilterLengthSlider->setMaximumPosition(750);
    m_FilterLengthSlider->setMinimum(0);

    m_FilterDepthSlider->setMaximum(DEPTH_MAX);
    m_FilterDepthSlider->setMaximumPosition(1000);
    m_FilterDepthSlider->setMinimum(0);

    m_DepthTypeComboBox = new QComboBox;
    m_DepthTypeComboBox->addItem("Start Depth");
    m_DepthTypeComboBox->addItem("End Depth");
    m_DepthTypeComboBox->addItem("Depth Change");

    qDebug() << m_FilterLengthSlider->maximumValue() << m_FilterLengthSlider->maximumPosition() << m_FilterLengthSlider->maximum();

    m_FilterDispSlider->setMaximum(DISPLACEMENT_MAX);
    m_FilterDispSlider->setMinimum(0);

    rightLayout->addWidget(new QLabel("Length Filter:"));
    rightLayout->addWidget(m_FilterLengthSlider);
    rightLayout->addWidget(new QLabel("Displacement Filter:"));
    rightLayout->addWidget(m_FilterDispSlider);
    rightLayout->addWidget(new QLabel("Depth Filter:"));
    rightLayout->addWidget(m_FilterDepthSlider);
    rightLayout->addWidget(m_DepthTypeComboBox);

    connect(m_FilterDepthSlider, SIGNAL(valuesChanged(int, int)), this, SIGNAL(depthFilterChanged(int, int)));
    connect(m_DepthTypeComboBox, SIGNAL(currentIndexChanged(int)), this, SIGNAL(depthFilterTypeChanged(int)));
    connect(m_FilterDispSlider, SIGNAL(valuesChanged(int, int)), this, SIGNAL(displacementFilterChanged(int, int)));

    m_TabWidget->addTab(filteringWidget, "Filtering");
}

QIcon QueryDrawSetupWidget::drawColorMapIcon(ColorMap *map)
{

    static const int TYPE_SEQUENTIAL = 0;
    static const int TYPE_DIVERGENT = 1;
    static const int TYPE_QUALITIVE = 2;
    static const int TYPE_RAINBOW = 3;
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
                int num_rows;
                if (map->getNumberOfColors() > 16) {
                    num_rows = 5;
                    for (int y = 0; y < ICON_SIZE; y++) {
                        int column = double(x)/(double(ICON_SIZE)/5.0);
                        int row = double(y)/(double(ICON_SIZE)/5.0);
                        int value = column + row*5;
                        float s = ColorMap::normalize(0.0, double(map->getNumberOfColors()-1), 0.0, 1.0, double(value));
                        if (s > 1.0f) {
                            color = QColor(255,255,255);
                        } else {
                            color = map->getColor(s);
                        }
                        iconImage.setPixelColor(x, y, color);
                    }
                } else if (map->getNumberOfColors() > 9) {
                    num_rows = 4;
                    for (int y = 0; y < ICON_SIZE; y++) {
                        int column = double(x)/(double(ICON_SIZE)/4.0);
                        int row = double(y)/(double(ICON_SIZE)/4.0);
                        int value = column + row*4;
                        float s = ColorMap::normalize(0.0, double(map->getNumberOfColors()-1), 0.0, 1.0, double(value));
                        if (s > 1.0f) {
                            color = QColor(255,255,255);
                        } else {
                            color = map->getColor(s);
                        }
                        iconImage.setPixelColor(x, y, color);
                    }
                } else if (map->getNumberOfColors() > 4) {
                    num_rows = 3;
                    for (int y = 0; y < ICON_SIZE; y++) {
                        int column = double(x)/(double(ICON_SIZE)/3.0);
                        int row = double(y)/(double(ICON_SIZE)/3.0);
                        int value = column + row*3;
                        float s = ColorMap::normalize(0.0, double(map->getNumberOfColors()-1), 0.0, 1.0, double(value));
                        if (s > 1.0f) {
                            color = QColor(255,255,255);
                        } else {
                            color = map->getColor(s);
                        }
                        iconImage.setPixelColor(x, y, color);
                    }
                } else if (map->getNumberOfColors() > 3) {
                    num_rows = 2;
                    for (int y = 0; y < ICON_SIZE; y++) {
                        int column = double(x)/(double(ICON_SIZE)/2.0);
                        int row = double(y)/(double(ICON_SIZE)/2.0);
                        int value = column + row*2;
                        float s = ColorMap::normalize(0.0, double(map->getNumberOfColors()-1), 0.0, 1.0, double(value));
                        color = map->getColor(s);
                        iconImage.setPixelColor(x, y, color);
                    }
                } else {
                    num_rows = 1;
                    float s = ColorMap::normalize(0.0, double(ICON_SIZE), 0.0, 1.0, double(x));
                    color = map->getColor(s);
                    for (int y = 0; y < ICON_SIZE; y++) {
                        iconImage.setPixelColor(x, y, color);
                    }
                }
            } else if (map->getType() == TYPE_SEQUENTIAL) {
                float s = ColorMap::normalize(0.0, double(ICON_SIZE), 0.0, 1.0, double(x));
                color = map->getColor(s);
                for (int y = 0; y < ICON_SIZE; y++) {
                    iconImage.setPixelColor(x, y, color);
                }

            }
        }
        QColor black = QColor(0,0,0);
        for (int i = 0; i < ICON_SIZE; i++) {
            iconImage.setPixelColor(i, 0, black);
            iconImage.setPixelColor(i, ICON_SIZE-1, black);
            iconImage.setPixelColor(0, i, black);
            iconImage.setPixelColor(ICON_SIZE-1, i, black);
        }
        return QIcon(QPixmap::fromImage(iconImage));
}

void QueryDrawSetupWidget::setupLengthColorArea()
{
    m_VarLengthColorWidget = new QWidget;
    QVBoxLayout *mainLayout = new QVBoxLayout;

    QLabel *titleLabel = new QLabel("Color mapping variable: <b>Length</b>");
    m_VarLengthInclZCheckBox = new QCheckBox("Include Z: ");
    QPushButton *applyButton = new QPushButton("Apply");

    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(m_VarLengthInclZCheckBox);
    mainLayout->addWidget(applyButton);

    connect(applyButton, SIGNAL(clicked()), this, SLOT(lengthColorChanged()));

    m_VarLengthColorWidget->setLayout(mainLayout);

}

void QueryDrawSetupWidget::setupDisplacementColorArea()
{
    m_VarDispColorWidget = new QWidget;
    QVBoxLayout *mainLayout = new QVBoxLayout;

    QLabel *titleLabel = new QLabel("Color mapping variable: <b>Displacement</b>");
    m_VarDispInclZCheckBox = new QCheckBox("Include Z: ");
    m_VarDispT1SpinBox = new QSpinBox;
    m_VarDispT2SpinBox = new QSpinBox;
    m_VarDispT1SpinBox->setRange(0, 50);
    m_VarDispT2SpinBox->setRange(0, 50);
    m_VarDispT1SpinBox->setValue(0);
    m_VarDispT2SpinBox->setValue(50);
    QLabel *spinboxTitleLabel = new QLabel("Displacement time-range");
    QLabel *t1SpinBoxLabel = new QLabel("Start step: ");
    QLabel *t2SpinBoxLabel = new QLabel("Final step: ");
    QHBoxLayout *spinBoxLayout = new QHBoxLayout;
    spinBoxLayout->addWidget(t1SpinBoxLabel);
    spinBoxLayout->addWidget(m_VarDispT1SpinBox);
    spinBoxLayout->addWidget(t2SpinBoxLabel);
    spinBoxLayout->addWidget(m_VarDispT2SpinBox);

    QPushButton *applyButton = new QPushButton("Apply");

    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(m_VarDispInclZCheckBox);
    mainLayout->addWidget(spinboxTitleLabel);
    mainLayout->addLayout(spinBoxLayout);
    mainLayout->addWidget(applyButton);

    connect(applyButton, SIGNAL(clicked()), this, SLOT(displacementColorChanged()));

    m_VarDispColorWidget->setLayout(mainLayout);
}

void QueryDrawSetupWidget::setupVelocityColorArea()
{
    m_VarVelocityColorWidget = new QWidget;
    QVBoxLayout *mainLayout = new QVBoxLayout;

    QLabel *titleLabel = new QLabel("Color mapping variable: <b>Velocity</b>");
    m_VarVelocityInclZCheckBox = new QCheckBox("Include Z: ");
    m_VarVelocityMapDirCheckBox = new QCheckBox("Map direction to hue");
    m_VarVelocityAvgRadioBtn = new QRadioButton("Average velocity");
    m_VarVelocityPerStepRadioBtn = new QRadioButton("Per timestep velocity");
    m_VarVelocityBtnGroup = new QButtonGroup;
    m_VarVelocityBtnGroup->setExclusive(true);
    m_VarVelocityBtnGroup->addButton(m_VarVelocityAvgRadioBtn);
    m_VarVelocityBtnGroup->addButton(m_VarVelocityPerStepRadioBtn);

    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(m_VarVelocityInclZCheckBox);
    mainLayout->addWidget(m_VarVelocityMapDirCheckBox);
    mainLayout->addWidget(m_VarVelocityAvgRadioBtn);
    mainLayout->addWidget(m_VarVelocityPerStepRadioBtn);

    QPushButton *applyButton = new QPushButton("Apply");
    connect(applyButton, SIGNAL(clicked()), this, SLOT(velocityColorChanged()));
    connect(m_VarVelocityMapDirCheckBox, SIGNAL(stateChanged(int)), this, SLOT(velocityMapHueChanged(int)));

    mainLayout->addWidget(applyButton);

    m_VarVelocityColorWidget->setLayout(mainLayout);


}

void QueryDrawSetupWidget::setupDepthColorArea()
{
    m_VarDepthColorWidget = new QWidget;
    QVBoxLayout *mainLayout = new QVBoxLayout;
    m_VarDepthColorWidget->setLayout(mainLayout);

    QLabel *titleLabel = new QLabel("Color mapping variable: <b>Depth</b>");

    m_VarDepthBtnGroup = new QButtonGroup;
    m_VarDepthBtnGroup->setExclusive(true);
    m_VarDepthCurCheckBox = new QCheckBox("Time-Dependent depth");
    m_VarDepthStartCheckBox = new QCheckBox("Start Depth");
    m_VarDepthEndCheckBox = new QCheckBox("End Depth");
    m_VarDepthChangeCheckBox = new QCheckBox("Change in depth");

    m_VarDepthBtnGroup->addButton(m_VarDepthCurCheckBox);
    m_VarDepthBtnGroup->addButton(m_VarDepthStartCheckBox);
    m_VarDepthBtnGroup->addButton(m_VarDepthEndCheckBox);
    m_VarDepthBtnGroup->addButton(m_VarDepthChangeCheckBox);

    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(m_VarDepthCurCheckBox);
    mainLayout->addWidget(m_VarDepthStartCheckBox);
    mainLayout->addWidget(m_VarDepthEndCheckBox);
    mainLayout->addWidget(m_VarDepthChangeCheckBox);

    QPushButton *applyButton = new QPushButton("Apply");
    mainLayout->addWidget(applyButton);

    connect(applyButton, SIGNAL(clicked()), this, SLOT(depthColorChanged()));
}

void QueryDrawSetupWidget::setupCurvatureColorArea()
{
    m_VarCurvatureColorWidget = new QWidget;
    QVBoxLayout *mainLayout = new QVBoxLayout;
    m_VarCurvatureColorWidget->setLayout(mainLayout);

    QLabel *titleLabel = new QLabel("Color mapping variable: <b>Curvature</b>");

    mainLayout->addWidget(titleLabel);


    QPushButton *applyButton = new QPushButton("Apply");
    mainLayout->addWidget(applyButton);

    connect(applyButton, SIGNAL(clicked()), this, SLOT(curvatureColorChanged()));
}

void QueryDrawSetupWidget::setupBearingColorArea()
{
    m_VarBearingColorWidget = new QWidget;
    QVBoxLayout *mainLayout = new QVBoxLayout;
    m_VarBearingColorWidget->setLayout(mainLayout);

    QLabel *titleLabel = new QLabel("Color mapping variable: <b>Bearing</b>");

    mainLayout->addWidget(titleLabel);


    QPushButton *applyButton = new QPushButton("Apply");
    mainLayout->addWidget(applyButton);

    connect(applyButton, SIGNAL(clicked()), this, SLOT(bearingColorChanged()));
}

void QueryDrawSetupWidget::setupAtomColorArea()
{
    m_VarAtomColorWidget = new QWidget;
    QVBoxLayout *mainLayout = new QVBoxLayout;
    m_VarAtomColorWidget->setLayout(mainLayout);

    QLabel *titleLabel = new QLabel("Color mapping variable: <b>Atom</b>");
    m_VarAtomBtnGroup = new QButtonGroup;
    m_VarAtomBtnGroup->setExclusive(true);
    m_VarAtomSingleHueCheckBox = new QRadioButton("Single Hue");
    m_VarAtomMultiHueCheckBox = new QRadioButton("Multi Hue");
    m_VarAtomSingleHueCheckBox->setChecked(true);
    m_VarAtomBtnGroup->addButton(m_VarAtomSingleHueCheckBox);
    m_VarAtomBtnGroup->addButton(m_VarAtomMultiHueCheckBox);
    m_VarAtomNumClassesLabel = new QLabel("Number of classes: <b>NaN</b>");

    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(m_VarAtomSingleHueCheckBox);
    mainLayout->addWidget(m_VarAtomMultiHueCheckBox);
    mainLayout->addWidget(m_VarAtomNumClassesLabel);

    QPushButton *applyButton = new QPushButton("Apply");
    mainLayout->addWidget(applyButton);

    connect(applyButton, SIGNAL(clicked()), this, SLOT(atomColorChanged()));
}

void QueryDrawSetupWidget::setupResidueColorArea()
{
    m_VarResidueColorWidget = new QWidget;
    QVBoxLayout *mainLayout = new QVBoxLayout;
    m_VarResidueColorWidget->setLayout(mainLayout);

    QLabel *titleLabel = new QLabel("Color mapping variable: <b>Residue</b>");
    m_VarResidueBtnGroup = new QButtonGroup;
    m_VarResidueBtnGroup->setExclusive(true);
    m_VarResidueSingleHueCheckBox = new QRadioButton("Single Hue");
    m_VarResidueMultiHueCheckBox = new QRadioButton("Multi Hue");
    m_VarResidueSingleHueCheckBox->setChecked(true);
    m_VarResidueBtnGroup->addButton(m_VarResidueSingleHueCheckBox);
    m_VarResidueBtnGroup->addButton(m_VarResidueMultiHueCheckBox);
    m_VarResidueNumClassesLabel = new QLabel("Number of classes: <b>NaN</b>");

    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(m_VarResidueSingleHueCheckBox);
    mainLayout->addWidget(m_VarResidueMultiHueCheckBox);
    mainLayout->addWidget(m_VarResidueNumClassesLabel);

    QPushButton *applyButton = new QPushButton("Apply");
    mainLayout->addWidget(applyButton);

    connect(applyButton, SIGNAL(clicked()), this, SLOT(residueColorChanged()));
}

void QueryDrawSetupWidget::setupMoleculeColorArea()
{
    m_VarMoleculeColorWidget = new QWidget;
    QVBoxLayout *mainLayout = new QVBoxLayout;
    m_VarMoleculeColorWidget->setLayout(mainLayout);

    QLabel *titleLabel = new QLabel("Color mapping variable: <b>Molecule</b>");

    mainLayout->addWidget(titleLabel);


    QPushButton *applyButton = new QPushButton("Apply");
    mainLayout->addWidget(applyButton);

    connect(applyButton, SIGNAL(clicked()), this, SLOT(moleculeColorChanged()));
}

void QueryDrawSetupWidget::setupClusterColorArea()
{
    m_VarClusterColorWidget = new QWidget;
    QVBoxLayout *mainLayout = new QVBoxLayout;
    m_VarClusterColorWidget->setLayout(mainLayout);

    QLabel *titleLabel = new QLabel("Color mapping variable: <b>Cluster</b>");

    mainLayout->addWidget(titleLabel);


    QPushButton *applyButton = new QPushButton("Apply");
    mainLayout->addWidget(applyButton);

    connect(applyButton, SIGNAL(clicked()), this, SLOT(clusterColorChanged()));
}
