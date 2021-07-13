#include "euclideanviewwindow.h"
#include "mathfuncs.h"
#include "IO/filehandler.h"
#include "Data/vertex.h"
#include "Data/particle.h"
#include "Data/globalvars.h"
#include <QDebug>
#include <QTime>

EuclideanViewWindow::EuclideanViewWindow(QWidget *parent)
{
    timer.start();
    setMinimumSize(1200,1200);
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Ignored);
    setWindowState(Qt::WindowMaximized);
}

void EuclideanViewWindow::CameraKeyController(QString key)
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
    } else if (QString::compare(key, "l", Qt::CaseInsensitive) == 0) {
        renderScreen();
        update();
    }
}

void EuclideanViewWindow::changeBackgroundColor(QColor color)
{
    m_BackGroundColor = color;
    update();
}

void EuclideanViewWindow::changeAA(int state)
{
    qDebug() << "EuclideanViewWindow::changeAA(" << state << ")";
    if (state > 0) m_isAAEnabled = true;
    else m_isAAEnabled = false;
    update();
}

void EuclideanViewWindow::resetCamera()
{
    m_Camera.resetView();
    m_Camera.translate(QVector3D(m_SimulationBox.x()/2, m_SimulationBox.y()/2, 150));
    update();
}

void EuclideanViewWindow::setFrameNumber(int t)
{
    m_CurrentFrame = t;
    update();
}

void EuclideanViewWindow::initializeGL()
{
    float fov = 45.0f;
    float aspect = (float)this->width()/(float)this->height();
    float mNear = 1.0f;
    float mFar = 10000.0f;

    QMatrix4x4 defaultView;
    QVector3D eye(0.0,0.0,20.0);
    QVector3D center(0.0,0.0,0.0);
    QVector3D up(0.0,1.0,0.0);
    defaultView.lookAt(eye,center,up);

    m_SimulationBox = GlobalVars::g_SimulationBox;
    QVector3D m_Translation = m_SimulationBox/(2.0);

    m_WorldMatrix = QMatrix4x4();
    m_WorldMatrix.setToIdentity();
    //m_WorldMatrix.translate(m_Translation);

    m_Camera = EuclideanViewCamera();
    m_Camera.setDefaultView(defaultView);
    m_Camera.translate(QVector3D(m_SimulationBox.x()/2, m_SimulationBox.y()/2, 150));

    initializeOpenGLFunctions();
    glClearColor(0, 0, 0, 1);
    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_LINE_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    //glDisable(GL_CULL_FACE);
    qDebug() << "EuclideanViewWindow:: initShaders()";
    initShaders();
    qDebug() << "EuclideanViewWindow:: createVertexBuffer()";
    createVertexBuffer();
    m_Projection.setToIdentity();
    m_Projection.perspective(fov,aspect, mNear,mFar);
    qDebug() << "EuclideanViewWindow:: initializeGLFinished()";

}

void EuclideanViewWindow::resizeGL(int w, int h)
{
    float fov = 45.0f;
    qreal aspect = qreal(w) / qreal(h ? h : 1);
    float mFar = 10000.0f;
    m_Projection.setToIdentity();
    m_Projection.perspective(45.0f, aspect, 0.01f, 1000.0f);


    // Reset projection
    //camera->getProjection().setToIdentity();

    // Set perspective projection
}

void EuclideanViewWindow::paintGL()
{

    initializeOpenGLFunctions();
    glClearColor(m_BackGroundColor.redF(), m_BackGroundColor.greenF(), m_BackGroundColor.blueF(), 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    float fov = 45.0f;
    float aspect = (float)this->width()/(float)this->height();
    float mNear = 1.0f;
    float mFar = 10000.0f;


    for (int i = 0; i < GlobalVars::g_DrawQueryList.size(); i++) {
        if (GlobalVars::g_DrawQueryList[i].isDrawPath()) {
            int bufferIndex = GlobalVars::g_DrawQueryList[i].getDataBufferIndex();
            drawPathlines(i,bufferIndex);
        }
        if (GlobalVars::g_DrawQueryList[i].isDrawParticle()) {
            int bufferIndex = GlobalVars::g_DrawQueryList[i].getDataBufferIndex();
            //drawPoints(i,bufferIndex, m_CurrentFrame);
            drawBillboardPoints(i,bufferIndex, m_CurrentFrame);
        }
    }
    framecount++;
    time += timer.elapsed();
    if (time >= 1000) {
        qDebug() << "Euclidean framecount: " << framecount << " FPS: " << framecount / ((double)time/1000.0);
        time = 0;
        framecount = 0;
    }
    timer.restart();

}

void EuclideanViewWindow::initShaders()
{
    m_Program = new QOpenGLShaderProgram();
    if (!m_Program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/euclideanLineVShader.glsl"))
        close();
    if (!m_Program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/polarfshader.glsl"))
       close();
   if (!m_Program->link())
       close();
   if (!m_Program->bind())
       close();
    m_Program->release();

    m_PointShaderProgram = new QOpenGLShaderProgram();
    if (!m_PointShaderProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/PointVShader.glsl"))
        close();
    if (!m_PointShaderProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/PointFShader.glsl"))
       close();
    if (!m_PointShaderProgram->link())
       close();
    if (!m_PointShaderProgram->bind())
       close();
    m_PointShaderProgram->release();

    m_BillboardPointShaderProgram = new QOpenGLShaderProgram();
    if (!m_BillboardPointShaderProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/BillboardCircleVShader.glsl"))
        close();
    if (!m_BillboardPointShaderProgram->addShaderFromSourceFile(QOpenGLShader::Geometry, ":/shaders/BillboardCircleGShader.glsl"))
        close();
    if (!m_BillboardPointShaderProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/BillboardCircleFShader.glsl"))
       close();
    if (!m_BillboardPointShaderProgram->link())
       close();
    if (!m_BillboardPointShaderProgram->bind())
       close();
    m_BillboardPointShaderProgram->release();
}

void EuclideanViewWindow::createVertexBuffer()
{
    makeCurrent();
    for (int buffnum = 0; buffnum < testfh->m_VertexList.size(); buffnum++) {
        QOpenGLBuffer buffer;
        buffer.create();
        m_VertexBufferList.append(buffer);

        int NUMBER_OF_ATOMS = testfh->m_VertexList[buffnum].length();
        int NUMBER_OF_FRAMES = testfh->m_VertexList[buffnum].first().length();
        int FRAME_OFFSET = NUMBER_OF_FRAMES * sizeof(Vertex);

        buffer.bind();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        buffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
        buffer.allocate(sizeof(Vertex) * NUMBER_OF_ATOMS * NUMBER_OF_FRAMES);

        for (int i = 0; i < NUMBER_OF_ATOMS; i++) {
            QVector<Vertex> data;
            for (int j = 0; j < NUMBER_OF_FRAMES; ++j) {
                data.append(testfh->m_VertexList[buffnum][i][j]);
            }
            buffer.write(FRAME_OFFSET*i, data.constData(), FRAME_OFFSET);
        }
        buffer.release();
    }
    doneCurrent();
}

void EuclideanViewWindow::rewriteVertexBuffer(int buff_index)
{
    qDebug() << "EuclideanViewWindow:: Rewriting buffer index: " << buff_index;
    makeCurrent();
    m_VertexBufferList[buff_index].create();
    int NUMBER_OF_ATOMS = testfh->m_VertexList[buff_index].length();
    int NUMBER_OF_FRAMES = testfh->m_VertexList[buff_index].first().length();
    int FRAME_OFFSET = NUMBER_OF_FRAMES * sizeof(Vertex);

    m_VertexBufferList[buff_index].bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_VertexBufferList[buff_index].setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_VertexBufferList[buff_index].allocate(sizeof(Vertex)*NUMBER_OF_ATOMS*NUMBER_OF_FRAMES);
    for (int i = 0; i < NUMBER_OF_ATOMS; i++) {
        QVector<Vertex> data;
        for (int j = 0; j < NUMBER_OF_FRAMES; ++j) {
            data.append(testfh->m_VertexList[buff_index][i][j]);
        }
        m_VertexBufferList[buff_index].write(FRAME_OFFSET*i, data.constData(), FRAME_OFFSET);
    }
    m_VertexBufferList[buff_index].release();
    doneCurrent();
    update();
    qDebug() << "EuclideanViewWindow:: Finished rewriting buffer";
}

void EuclideanViewWindow::dataChanged(int queryIndex, int dataIndex)
{

}

void EuclideanViewWindow::renderScreen()
{
    setMinimumSize(4000,4000);
    update();
    QImage screenshot = grabFramebuffer();
    screenshot.save("test.jpg", "jpg", 100);
    setMinimumSize(1200,1200);
    update();
}

void EuclideanViewWindow::changeDepthFilter(int min, int max)
{
    if (m_DepthFilterType == 2) {
        double tmin = -10.0;
        double tmax = 10.0;
        double rmin = 0;
        double rmax = 1000;
        m_MinDepthFilter = tmin + (tmax-tmin)*(min-rmin)/(rmax-rmin);
        m_MaxDepthFilter = tmin + (tmax-tmin)*(max-rmin)/(rmax-rmin);
    } else {
        m_MinDepthFilter = (10.0)*(min)/(1000.0);
        m_MaxDepthFilter = (10.0)*(max)/(1000.0);
    }

    qDebug() << "EuclideanViewWindow::changeDepthFilter(" << min << max << ") Min Depth: " << m_MinDepthFilter << "Max Depth: " << m_MaxDepthFilter;
    update();
}

void EuclideanViewWindow::changeDepthFilterType(int type)
{
    m_DepthFilterType = type;
    update();
}

void EuclideanViewWindow::changeDisplacementFilter(int min, int max)
{
    double tmin = 0.0;
    double tmax = 10.0;
    double rmin = 0;
    double rmax = 1000;
    m_MinDisplacementFilter = tmin + (tmax-tmin)*(min-rmin)/(rmax-rmin);
    m_MaxDisplacementFilter = tmin + (tmax-tmin)*(max-rmin)/(rmax-rmin);
    qDebug() << "EuclideanViewWindow::changeDisplacementFilter(" << min << max << ") Min Disp: " << m_MinDisplacementFilter << "Max Disp: " << m_MaxDisplacementFilter;
    update();
}

void EuclideanViewWindow::testDraw(int queryIndex)
{

}

void EuclideanViewWindow::drawPathlines(int queryIndex, int vertexListIndex)
{
    makeCurrent();
    int NUMBER_OF_ATOMS = testfh->m_VertexList[vertexListIndex].length();
    int NUMBER_OF_FRAMES = testfh->m_VertexList[vertexListIndex].first().length();
    int OFFSET = sizeof(Vertex);
    int TUPLE_SIZE = 3;

    m_Program->bind();
    m_VertexBufferList[vertexListIndex].bind();
    if (m_isAAEnabled) glEnable(GL_LINE_SMOOTH);
    else glDisable(GL_LINE_SMOOTH);

    m_Program->enableAttributeArray(0);
    m_Program->enableAttributeArray(1);

    m_Program->setAttributeBuffer(0, GL_FLOAT,
                                  Vertex::posOffset(),
                                  TUPLE_SIZE,
                                  OFFSET);
    m_Program->setAttributeBuffer(1, GL_FLOAT,
                                  Vertex::colorOffset(),
                                  TUPLE_SIZE,
                                  OFFSET);

    m_Program->setUniformValue("modelToWorld",
                                   m_WorldMatrix);
    m_Program->setUniformValue("worldToCamera",
                                   m_Camera.toMatrix());
    m_Program->setUniformValue("cameraToView",
                                   m_Projection);

    int isContext = int(GlobalVars::g_DrawQueryList[queryIndex].isContext());

    m_Program->setUniformValue("contextVal", isContext);

    glLineWidth(GlobalVars::g_DrawQueryList[queryIndex].getPathlineThickness());

    int parent_query_index = GlobalVars::g_DrawQueryList[queryIndex].getParentQueryIndex();
    for (int i = 0; i < NUMBER_OF_ATOMS; i++)
    {
        if (!GlobalVars::g_DrawQueryList[queryIndex].isNotQuery()) {
            if (GlobalVars::g_DrawQueryList[queryIndex].getBit(i)) {
                double depth;
                if (m_DepthFilterType == 0) depth = testfh->m_VertexList[vertexListIndex][i][0].getPosition().z();
                else if (m_DepthFilterType == 1) depth = testfh->m_VertexList[vertexListIndex][i].back().getPosition().z();
                else {
                    double start_depth = GlobalVars::g_FileHandler->m_VertexList[vertexListIndex][i].front().getPosition().z();
                    double end_depth = GlobalVars::g_FileHandler->m_VertexList[vertexListIndex][i].back().getPosition().z();
                    depth = start_depth - end_depth;
                }
                double displacement = testfh->m_ParticleList[vertexListIndex][i].getParticleDisplacement();

                if (depth > m_MinDepthFilter && depth < m_MaxDepthFilter) {
                    if (displacement > m_MinDisplacementFilter && displacement < m_MaxDisplacementFilter) {
                        glDrawArrays(GL_LINE_STRIP, i*51, 51);
                    }
                }
            }
        } else if (!GlobalVars::g_DrawQueryList[queryIndex].getBit(i) && GlobalVars::g_DrawQueryList[parent_query_index].getBit(i)) {
                glDrawArrays(GL_LINE_STRIP, i*51, 51);
        }
    }
    m_Program->release();
    m_VertexBufferList[vertexListIndex].release();
}

void EuclideanViewWindow::drawPoints(int queryIndex, int vertexListIndex, int frameNumber)
{
    makeCurrent();
    int NUMBER_OF_ATOMS = testfh->m_VertexList[vertexListIndex].length();
    int NUMBER_OF_FRAMES = testfh->m_VertexList[vertexListIndex].first().length();
    int OFFSET = sizeof(Vertex);
    int TUPLE_SIZE = 3;
    int FRAME_OFFSET = frameNumber * OFFSET;

    m_PointShaderProgram->bind();
    m_VertexBufferList[vertexListIndex].bind();
    m_PointShaderProgram->enableAttributeArray(0);
    m_PointShaderProgram->enableAttributeArray(1);
    m_PointShaderProgram->setAttributeBuffer(0, GL_FLOAT, FRAME_OFFSET + Vertex::colorOffset(), 3, sizeof(Vertex) * NUMBER_OF_FRAMES);
    m_PointShaderProgram->setAttributeBuffer(1, GL_FLOAT, FRAME_OFFSET + Vertex::posOffset(), 3, sizeof(Vertex) * NUMBER_OF_FRAMES);

    m_PointShaderProgram->setUniformValue("modelToWorld", m_WorldMatrix);
    m_PointShaderProgram->setUniformValue("worldToCamera", m_Camera.toMatrix());
    m_PointShaderProgram->setUniformValue("cameraToView", m_Projection);
    int isContext = int(GlobalVars::g_DrawQueryList[queryIndex].isContext());
    m_Program->setUniformValue("contextVal", isContext);

    float point_size = GlobalVars::g_DrawQueryList[queryIndex].getParticlePointSize();
    glPointSize(point_size);

    int parent_query_index = GlobalVars::g_DrawQueryList[queryIndex].getParentQueryIndex();
    for (int i = 0; i < NUMBER_OF_ATOMS; i++)
    {
        if (!GlobalVars::g_DrawQueryList[queryIndex].isNotQuery()) {
            if (GlobalVars::g_DrawQueryList[queryIndex].getBit(i)) {
                double depth = testfh->m_VertexList[vertexListIndex][i][0].getPosition().z();
                if (depth > m_MinDepthFilter && depth < m_MaxDepthFilter) {
                    glDrawArrays(GL_POINTS, i, 1);
                }
            }
        } else if (!GlobalVars::g_DrawQueryList[queryIndex].getBit(i) && GlobalVars::g_DrawQueryList[parent_query_index].getBit(i)) {
                glDrawArrays(GL_POINTS, i, 1);
        }
    }


    //glDrawArrays(GL_POINTS,0, NUMBER_OF_ATOMS);
    m_VertexBufferList[vertexListIndex].release();
    m_PointShaderProgram->release();
    doneCurrent();
}

void EuclideanViewWindow::drawBillboardPoints(int queryIndex, int vertexListIndex, int frameNumber)
{
    makeCurrent();
    int NUMBER_OF_ATOMS = testfh->m_VertexList[vertexListIndex].length();
    int NUMBER_OF_FRAMES = testfh->m_VertexList[vertexListIndex].first().length();
    int OFFSET = sizeof(Vertex);
    int TUPLE_SIZE = 3;
    int FRAME_OFFSET = frameNumber * OFFSET;

    m_BillboardPointShaderProgram->bind();
    m_VertexBufferList[vertexListIndex].bind();
    m_BillboardPointShaderProgram->enableAttributeArray(0);
    m_BillboardPointShaderProgram->enableAttributeArray(1);
    m_BillboardPointShaderProgram->setAttributeBuffer(0, GL_FLOAT, FRAME_OFFSET + Vertex::colorOffset(), 3, sizeof(Vertex) * NUMBER_OF_FRAMES);
    m_BillboardPointShaderProgram->setAttributeBuffer(1, GL_FLOAT, FRAME_OFFSET + Vertex::posOffset(), 3, sizeof(Vertex) * NUMBER_OF_FRAMES);

    float point_size = GlobalVars::g_DrawQueryList[queryIndex].getParticlePointSize();
    point_size /= 10.0f;
    m_BillboardPointShaderProgram->setUniformValue("particle_size", point_size);
    m_BillboardPointShaderProgram->setUniformValue("gxl3d_ModelViewMatrix", m_Camera.toMatrix());
    m_BillboardPointShaderProgram->setUniformValue("gxl3d_ProjectionMatrix", m_Projection);
    int isContext = int(GlobalVars::g_DrawQueryList[queryIndex].isContext());
    m_BillboardPointShaderProgram->setUniformValue("contextVal", isContext);


    //glPointSize(5.0f);

    int parent_query_index = GlobalVars::g_DrawQueryList[queryIndex].getParentQueryIndex();
    for (int i = 0; i < NUMBER_OF_ATOMS; i++)
    {
        if (!GlobalVars::g_DrawQueryList[queryIndex].isNotQuery()) {
            if (GlobalVars::g_DrawQueryList[queryIndex].getBit(i)) {
                glDrawArrays(GL_POINTS, i, 1);
            }
        } else if (!GlobalVars::g_DrawQueryList[queryIndex].getBit(i) && GlobalVars::g_DrawQueryList[parent_query_index].getBit(i)) {
                glDrawArrays(GL_POINTS, i, 1);
        }
    }

    m_VertexBufferList[vertexListIndex].release();
    m_BillboardPointShaderProgram->release();
    doneCurrent();
}

void EuclideanViewWindow::mousePressEvent(QMouseEvent *event)
{
    m_mouseLocation = event->pos();
    m_isMouseMoving = true;
}

void EuclideanViewWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (m_isMouseMoving) {
        QPointF difference = event->pos() - m_mouseLocation;
        double MAX_ZOOM = 150.0;
        double MIN_ZOOM = 1.0;
        double currentZoom = m_Camera.getTranslation().z();
        difference *= MathFuncs::normalize(MIN_ZOOM, MAX_ZOOM, 0.01, 0.1, currentZoom);

        m_Camera.translate(QVector3D(-1*difference.x(),difference.y(),0.0));
        m_mouseLocation = event->pos();
        update();
    }
}

void EuclideanViewWindow::mouseReleaseEvent(QMouseEvent *event)
{
    m_isMouseMoving = false;
}

void EuclideanViewWindow::wheelEvent(QWheelEvent *event)
{
    int SCROLL_SPEED = 5;
    int direction = 1;
    if (event->delta() > 0) direction = -1;
    m_Camera.translate(m_Camera.forward()*direction*SCROLL_SPEED);
    update();
}
