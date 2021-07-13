#ifndef POLARVIEWWINDOW_H
#define POLARVIEWWINDOW_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLWindow>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QKeyEvent>
#include <QtMath>
#include <QOpenGLExtraFunctions>

#include "ColorMap/colormap.h"
#include "euclideanviewcamera.h"
#include "IO/filehandler.h"
#include "Data/vertex.h"
#include <QTime>

class PolarViewWindow : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    PolarViewWindow(QWidget *parent = nullptr);
    void setFileHandler(FileHandler *fh);
public slots:
    void CameraKeyController(QString key);
    void changeBackgroundColor(QColor color);
    void changeGUISize(int size);
    void changeTrajectorySize(int size);
    void setGUIVisible(int state);
    void setGridVisible(int state);
    void setContextVisible(int state);
    void setLockAngle(int state);
    void setLengthFilterType(bool checked);
    void changeAA(int state);
    void changeFlatPerspective(int state);
    void changeDepthFilter(int min, int max);
    void changeDepthFilterType(int type);
    void changeDisplacementFilter(int min, int max);

    void rewriteVertexBuffer(int queryIndex, int dataIndex);
    void colorChanged(int dataIndex);
    void queryChanged();
    void renderScreen();
signals:
    void dataChanged(int bufferIndex);
    void angleFilterChanged(double min, double max);
    void frameRendered();
protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
    void initShaders();
    void createVertexBuffer();
    void createVertexBuffer(int queryIndex, int vertexListIndex);
    void createGUIBuffer();
    void createCircleBuffer();
    void drawPaths();
    void drawRedPaths();
    void drawGUI();
    void drawGrid();
    void changeFilter();
    void keyReleaseEvent(QKeyEvent *e) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

private:
    void calcMaxPathLength();
    void updateColors();

    QMatrix4x4 m_WorldMatrix;
    QMatrix4x4 m_GUIMatrix;
    QMatrix4x4 m_Projection;
    FileHandler *m_FileHandler;
    EuclideanViewCamera *m_Camera;

    QOpenGLShaderProgram *m_PolarShdrPrgrm;
    QOpenGLShaderProgram *m_PolarGUIShdrPrgrm;
    QOpenGLShaderProgram *m_PolarShdrPrgrmScnd;
    QOpenGLShaderProgram *m_GUICircleShdrPrgm;
    QOpenGLShaderProgram *m_LineShdrPrgm;

    QOpenGLBuffer m_VertexBuffer;
    QOpenGLBuffer m_GUIBuffer;
    QOpenGLBuffer m_CircleBuffer;

    QColor m_BackgroundColor = QColor(Qt::black);

    ColorMap *m_ColorMap;
    double m_MinAngleFilter = 0;
    double m_MaxAngleFilter = M_PI_2;
    double m_AngleDiff;

    int m_SimLength;
    int m_QueryIndex;
    int m_DataIndex;
    double m_MaxPathLength = 6;
    double m_MaxPathLengthFilter= 5;
    double m_MaxDepthLengthFilter = 10;

    float m_pathLineWidth = 1.0f;
    float m_GUILineWidth = 4.0f;

    bool m_isGUIDrawn = true;
    bool m_isGridDrawn = false;
    bool m_isAAEnabled = false;
    bool m_isFlatPerspective = false;
    bool m_isContextDrawn = false;
    bool m_hasData = false;
    bool m_SomethingChanged= false;
    bool m_isMaxLengthFilter = true;
    bool m_isLockAngle = false;
    bool m_MinAngleChanging;
    bool m_MaxAngleChanging;
    bool m_LengthFilterChanging;
    bool m_filterChanged;
    bool m_MouseMoving = false;
    bool m_LeftMouse = false;
    bool m_RightMouse = false;

    QTime timer;
    int framecount = 0;
    int time = 0;
    int m_CurrentFrame = 0;

    double m_MinDepthFilter = 0.0;
    double m_MaxDepthFilter = 10.0;
    int m_DepthFilterType = 0;
    double m_MinDisplacementFilter = 0.0;
    double m_MaxDisplacementFilter = 0.0;
};

#endif // POLARVIEWWINDOW_H
