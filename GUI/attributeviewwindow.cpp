#include "attributeviewwindow.h"
#include "IO/filehandler.h"
#include "Data/vertex.h"
#include "Data/particle.h"
#include "Data/globalvars.h"
#include <QDebug>


AttributeViewWindow::AttributeViewWindow(int query_index, int data_index, int mode, QWidget *parent)
{
    m_QueryIndex = query_index;
    m_DataIndex = data_index;
    m_Mode = mode;
}

void AttributeViewWindow::CameraKeyController(QString key)
{
    if (QString::compare(key, "w", Qt::CaseInsensitive) == 0) {
        m_Camera.translate(m_Camera.up());
        update();
    } else if (QString::compare(key, "a", Qt::CaseInsensitive) == 0) {
        m_Camera.translate(m_Camera.right()*-1);
        update();
    } else if (QString::compare(key, "s", Qt::CaseInsensitive) == 0) {
        m_Camera.translate(m_Camera.up()*-1);
        update();
    } else if (QString::compare(key, "d", Qt::CaseInsensitive) == 0) {
        m_Camera.translate(m_Camera.right());
        update();
    } else if (QString::compare(key, "up", Qt::CaseInsensitive) == 0) {
        m_Camera.translate(m_Camera.forward());
        update();
    } else if (QString::compare(key, "down", Qt::CaseInsensitive) == 0) {
        m_Camera.translate(m_Camera.forward()*-1);
        update();
    }
}

void AttributeViewWindow::initializeGL()
{
    float fov = 45.0f;
    float aspect = (float)this->width()/(float)this->height();
    float mNear = 1.0f;
    float mFar = 10000.0f;

    QMatrix4x4 defaultView;
    QVector3D eye(0.0,0.0,-20.0);
    QVector3D center(0.0,0.0,0.0);
    QVector3D up(0.0,1.0,0.0);
    defaultView.lookAt(eye,center,up);

    QVector3D m_Box = QVector3D(120.0,120.0,10.0);
    QVector3D m_Translation = m_Box/(-2.0);

    m_WorldMatrix = QMatrix4x4();
    m_WorldMatrix.setToIdentity();
    m_WorldMatrix.translate(m_Translation);

    m_Camera = EuclideanViewCamera();
    m_Camera.setDefaultView(defaultView);
    m_Camera.translate(QVector3D(0,0,-150));

    initializeOpenGLFunctions();
    glClearColor(0, 0, 0, 1);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    //glDisable(GL_CULL_FACE);
    qDebug() << "init shaders";
    initShaders();
    qDebug() << "create vertex Buffer";
    createVertexBuffers(m_QueryIndex, m_DataIndex, m_Mode);
    m_Projection.setToIdentity();
    m_Projection.perspective(fov,aspect, mNear,mFar);
}

void AttributeViewWindow::resizeGL(int w, int h)
{

}

void AttributeViewWindow::paintGL()
{

}

void AttributeViewWindow::initShaders()
{

}

void AttributeViewWindow::createVertexBuffers(int query_index, int data_index, int mode)
{
    int vertex_count = 0;
    int frame_count = testfh->m_VertexList[data_index].first().length();
    int num_queries = testfh->m_VertexList[data_index].length();
    for (int i = 0; i < num_queries; i++) {
        if (GlobalVars::g_DrawQueryList[query_index].getBit(i)) vertex_count++;
    }
    makeCurrent();
    m_VertexBuffer.create();
    m_VertexBuffer.bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_VertexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_VertexBuffer.allocate(sizeof(float) * vertex_count);
    QVector<float> data;
    for (int i = 0; i < num_queries; i++) {
        if (GlobalVars::g_DrawQueryList[query_index].getBit(i)) {
            float dist = 0.0f;
            for (int t = 1; t < frame_count; t++) {
                QVector3D a = testfh->m_VertexList[data_index][i][t-1].getPosition();
                QVector3D b = testfh->m_VertexList[data_index][i][t].getPosition();
                dist += a.distanceToPoint(b);
            }
            data.append(dist);
        }
    }
    std::sort(data.begin(), data.end());
    m_VertexBuffer.write(0, data.constData(), sizeof(float) * vertex_count);
    m_VertexBuffer.release();
    doneCurrent();
}


void AttributeViewWindow::drawHistogram(int query_index)
{

}
