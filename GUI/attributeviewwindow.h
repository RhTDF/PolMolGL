#ifndef ATTRIBUTEVIEWWINDOW_H
#define ATTRIBUTEVIEWWINDOW_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLWindow>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QKeyEvent>
#include <QtMath>

#include "euclideanviewcamera.h"

class FileHandler;

class AttributeViewWindow : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    AttributeViewWindow(int query_index, int data_index, int mode, QWidget *parent = nullptr);
    FileHandler *testfh;
public slots:
    void CameraKeyController(QString key);
protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
    void initShaders();
    void createVertexBuffers(int query_index, int data_index, int mode);

    void drawHistogram(int query_index);

private:
    QMatrix4x4 m_WorldMatrix;
    QMatrix4x4 m_Projection;

    EuclideanViewCamera m_Camera;
    QOpenGLShaderProgram *m_LineChartProgram;
    QOpenGLBuffer m_VertexBuffer;
    int m_QueryIndex;
    int m_DataIndex;
    int m_Mode;
};

#endif // ATTRIBUTEVIEWWINDOW_H
