#ifndef EUCLIDEANVIEWWINDOW_H
#define EUCLIDEANVIEWWINDOW_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLWindow>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QKeyEvent>
#include <QtMath>
#include <QColor>
#include <QTime>

#include "euclideanviewcamera.h"

class FileHandler;

class EuclideanViewWindow : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    EuclideanViewWindow(QWidget *parent = nullptr);
    FileHandler *testfh;

signals:
    void frameRendered();
public slots:
    void CameraKeyController(QString key);
    void changeBackgroundColor(QColor color);
    void changeAA(int state);
    void resetCamera();
    void setFrameNumber(int t);
    void rewriteVertexBuffer(int buff_index);
    void dataChanged(int queryIndex, int dataIndex);
    void renderScreen();
    void changeDepthFilter(int min, int max);
    void changeDepthFilterType(int type);
    void changeDisplacementFilter(int min, int max);
protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
    void initShaders();
    void createVertexBuffer();


    void testDraw(int queryIndex);

    void drawPathlines(int queryIndex, int vertexListIndex);
    void drawPoints(int queryIndex, int vertexListIndex, int frameNumber);
    void drawBillboardPoints(int queryIndex, int vertexListIndex, int frameNumber);
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
private:
    QMatrix4x4 m_WorldMatrix;
    QMatrix4x4 m_Projection;

    EuclideanViewCamera m_Camera;
    QOpenGLShaderProgram *m_Program;
    QOpenGLShaderProgram *m_PointShaderProgram;
    QOpenGLShaderProgram *m_BillboardPointShaderProgram;
    QOpenGLVertexArrayObject vbo;
    QVector<QOpenGLBuffer> m_VertexBufferList;
    QVector3D m_SimulationBox;
    QPoint m_mouseLocation;
    bool m_isMouseMoving = false;
    bool m_isAAEnabled = false;
    QColor m_BackGroundColor = QColor(Qt::black);

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

#endif // EUCLIDEANVIEWWINDOW_H
