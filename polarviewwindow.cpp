#include "polarviewwindow.h"

#include "Data/globalvars.h"
#include "Data/particle.h"

#include "mathfuncs.h"


PolarViewWindow::PolarViewWindow(QWidget *parent)
{
    timer.start();
    setMinimumSize(1200,1200);
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Ignored);

    setWindowState(Qt::WindowMaximized);
    m_SimLength = 51;
}

void PolarViewWindow::setFileHandler(FileHandler *fh)
{
    m_FileHandler = fh;
}

void PolarViewWindow::CameraKeyController(QString key)
{
    if (QString::compare(key, "l", Qt::CaseInsensitive) == 0) {
        renderScreen();
        update();
    }
}

void PolarViewWindow::changeBackgroundColor(QColor color)
{
    m_BackgroundColor = color;
    update();
}

void PolarViewWindow::changeGUISize(int size)
{
    m_GUILineWidth = (size+50)/100.0f;
    update();
}

void PolarViewWindow::changeTrajectorySize(int size)
{
    m_pathLineWidth = (size+50)/100.0f;
    update();
}

void PolarViewWindow::setGUIVisible(int state)
{
    m_isGUIDrawn = state == Qt::Checked;
    update();
}

void PolarViewWindow::setGridVisible(int state)
{
    m_isGridDrawn = state == Qt::Checked;
    update();
}

void PolarViewWindow::setContextVisible(int state)
{
    m_isContextDrawn = state == Qt::Checked;
    qDebug() << "PolarViewWindow::setContextVisible(" << state << ")";
    update();
}

void PolarViewWindow::setLockAngle(int state)
{
    m_isLockAngle = state == Qt::Checked;
    update();
}

void PolarViewWindow::setLengthFilterType(bool checked)
{
    m_isMaxLengthFilter = checked;
    update();
}

void PolarViewWindow::changeAA(int state)
{
    qDebug() << "PolarViewWindow::changeAA(" << state << ")";
    if (state > 0) m_isAAEnabled = true;
    else m_isAAEnabled = false;
    update();
}

void PolarViewWindow::changeFlatPerspective(int state)
{
    qDebug() << "PolarViewWindow::changeFlatPerspective(" << state << ")";
    if (state > 0) m_isFlatPerspective = true;
    else m_isFlatPerspective = false;
    update();
}

void PolarViewWindow::changeDepthFilter(int min, int max)
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

    qDebug() << "PolarViewWindow::changeDepthFilter(" << min << max << ") Min Depth: " << m_MinDepthFilter << "Max Depth: " << m_MaxDepthFilter;

    update();
}

void PolarViewWindow::changeDepthFilterType(int type)
{
    m_DepthFilterType = type;
    update();
}

void PolarViewWindow::changeDisplacementFilter(int min, int max)
{
    double tmin = 0.0;
    double tmax = 10.0;
    double rmin = 0;
    double rmax = 1000;
    m_MinDisplacementFilter = tmin + (tmax-tmin)*(min-rmin)/(rmax-rmin);
    m_MaxDisplacementFilter = tmin + (tmax-tmin)*(max-rmin)/(rmax-rmin);
    qDebug() << "PolarViewWindow::changeDisplacementFilter(" << min << max << ") Min Disp: " << m_MinDisplacementFilter << "Max Disp: " << m_MaxDisplacementFilter;
    update();
}

void PolarViewWindow::rewriteVertexBuffer(int queryIndex, int dataIndex)
{
    qDebug() << "PolarView :: rewriteVertexBuffer queryIndex: " << queryIndex << " dataIndex: " << dataIndex << " m_QueryIndex: " << m_QueryIndex;
    if (queryIndex != m_QueryIndex) createVertexBuffer(queryIndex, dataIndex);
    update();
}

void PolarViewWindow::colorChanged(int dataIndex)
{
    createVertexBuffer(m_QueryIndex, dataIndex);
    update();
}

void PolarViewWindow::queryChanged()
{
    update();
}

void PolarViewWindow::renderScreen()
{
    setMinimumSize(4000,4000);
    //m_isGUIDrawn = false;
    m_pathLineWidth = 3.0f;
    update();
    QImage screenshot = grabFramebuffer();
    screenshot.save("polarview.jpg", "jpg", 100);
    setMinimumSize(1200,1200);
    //m_isGUIDrawn = true;
    m_pathLineWidth = 1.0f;
    update();
}


void PolarViewWindow::initializeGL()
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

    QVector3D m_Box = QVector3D(120.0,120.0,10.0);
    QVector3D m_Translation = m_Box/(2.0);

    m_WorldMatrix = QMatrix4x4();
    m_WorldMatrix.setToIdentity();
    //m_WorldMatrix.translate(m_Translation);

    m_Camera = new EuclideanViewCamera();
    m_Camera->setDefaultView(defaultView);
    //m_Camera->translate(QVector3D(60,60,2));

    initializeOpenGLFunctions();
    glClearColor(0, 0, 0, 1);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

    m_PolarShdrPrgrm = new QOpenGLShaderProgram();
    m_PolarGUIShdrPrgrm = new QOpenGLShaderProgram();
    m_PolarShdrPrgrmScnd = new QOpenGLShaderProgram();
    m_GUICircleShdrPrgm = new QOpenGLShaderProgram();
    m_LineShdrPrgm = new QOpenGLShaderProgram();
    glClearColor(0.0, 0.0, 0.0, 1);

    initShaders();

    createCircleBuffer();
    createVertexBuffer(1,1);
    createGUIBuffer();



    m_Projection.setToIdentity();
    m_Projection.perspective(fov,aspect, mNear,mFar);
}

void PolarViewWindow::resizeGL(int w, int h)
{
//    float fov = 45.0f;
//    float aspect = (float)this->width()/(float)this->height();
//    float mNear = 1.0f;
//    float mFar = 1000.0f;
//    m_Projection.setToIdentity();
//    m_Projection.perspective(fov,aspect, mNear,mFar);
//    m_GUIMatrix = QMatrix4x4();
//    m_GUIMatrix.setToIdentity();
//    m_GUIMatrix.translate(QVector3D(w,h,10.0)/-2.0);
//    update();
}

void PolarViewWindow::paintGL()
{

    initializeOpenGLFunctions();

    glClearColor(m_BackgroundColor.redF(), m_BackgroundColor.greenF(), m_BackgroundColor.blueF(), 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    float fov = 45.0f;
    float aspect = (float)this->width()/(float)this->height();
    float mNear = 1.0f;
    float mFar = 10000.0f;
    m_Projection.setToIdentity();
    m_Projection.perspective(fov,aspect, mNear,mFar);

    if (m_hasData) drawPaths();

    if (m_isGUIDrawn) drawGUI();
    if (m_isGridDrawn) drawGrid();
    //glDisable(GL_BLEND);

    framecount++;
    time += timer.elapsed();
    if (time >= 1000) {
        qDebug() << "Polar framecount: " << framecount << " FPS: " << framecount / ((double)time/1000.0);
        time = 0;
        framecount = 0;
    }
    timer.restart();
}

void PolarViewWindow::initShaders()
{
    if (!m_PolarShdrPrgrm->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/polarvshader.glsl"))
        close();
    if (!m_PolarShdrPrgrm->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/polarfshader.glsl"))
       close();
   if (!m_PolarShdrPrgrm->link())
       close();
   if (!m_PolarShdrPrgrm->bind())
       close();
    m_PolarShdrPrgrm->release();

    if (!m_LineShdrPrgm->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/linevshader.glsl"))
        close();
    if (!m_LineShdrPrgm->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/linefshader.glsl"))
       close();
   if (!m_LineShdrPrgm->link())
       close();
   if (!m_LineShdrPrgm->bind())
       close();
    m_LineShdrPrgm->release();


    if (!m_PolarShdrPrgrmScnd->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/polarvshader.glsl"))
        close();
    if (!m_PolarShdrPrgrmScnd->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/polarfshader.glsl"))
       close();
   if (!m_PolarShdrPrgrmScnd->link())
       close();
   if (!m_PolarShdrPrgrmScnd->bind())
       close();
    m_PolarShdrPrgrmScnd->release();

    if (!m_GUICircleShdrPrgm->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/CircleVShader.glsl"))
        close();
    if (!m_GUICircleShdrPrgm->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/CircleFShader.glsl"))
       close();
   if (!m_GUICircleShdrPrgm->link())
       close();
   if (!m_GUICircleShdrPrgm->bind())
       close();
    m_GUICircleShdrPrgm->release();

    if (!m_PolarGUIShdrPrgrm->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/PolarGUIVShader2.glsl"))
        close();
    if (!m_PolarGUIShdrPrgrm->addShaderFromSourceFile(QOpenGLShader::Geometry, ":/shaders/PolarGUIGShader2.glsl"))
        close();
    if (!m_PolarGUIShdrPrgrm->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/PolarGUIFShader2.glsl"))
       close();
   if (!m_PolarGUIShdrPrgrm->link())
       close();
   if (!m_PolarGUIShdrPrgrm->bind())
       close();
    m_PolarGUIShdrPrgrm->release();
}

void PolarViewWindow::createVertexBuffer()
{
    QOpenGLWidget::makeCurrent();
    m_VertexBuffer.destroy();
    m_VertexBuffer.create();
    int nAtoms = m_FileHandler->m_VertexList[1].length();
    int nFrames = 51;
    int offset = nFrames*sizeof(Vertex);
    m_VertexBuffer.bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_VertexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_VertexBuffer.allocate(sizeof(Vertex)*nAtoms*nFrames);
    qDebug() << "2";
    for (int i = 0; i < nAtoms; i++) {
         QVector<Vertex> data;
         for (int j = 0; j < nFrames; ++j) {
             data.append(m_FileHandler->m_VertexList[1][i][j]);
         }
         m_VertexBuffer.write(offset*i, data.constData(), offset);
    }
//    int grayFrameCounter = 0;
//    for (int i = 0; i < nAtoms; i=i+13) {
//        if (m_FileHandler->m_VertexList[1][i][0].getColor().x() >= 1.0) {
//             QVector<Vertex> data;
//             for (int j = 0; j < nFrames; ++j) {
//                 data.append(m_FileHandler->m_VertexList[1][i][j]);
//             }
//             m_VertexBuffer.write(offset*grayFrameCounter, data.constData(), offset);
//             grayFrameCounter += 13;
//        }
//    }
    qDebug() << "3";
    m_VertexBuffer.release();
    doneCurrent();
}

void PolarViewWindow::createVertexBuffer(int queryIndex, int vertexListIndex)
{
    qDebug() << "Polar View:: createVertexBuffer() queryIndex: " << QString::number(queryIndex) << " vertexListIndex: " << QString::number(vertexListIndex);
    if (queryIndex > 1) {
        m_hasData = true;
        int numAtoms = 0;
        int numFrames = 51;
        m_QueryIndex = queryIndex;
        m_DataIndex = vertexListIndex;
        QVector<QVector<QVector3D>> data;
        for (int i = 0; i < GlobalVars::g_DrawQueryList[queryIndex].getBitArray().size(); i++)
        {
            //if (GlobalVars::g_DrawQueryList[queryIndex].getBit(i)) {
                QVector<QVector3D> particle;
                for (int j = 0; j < numFrames; ++j) {
                    particle.append(m_FileHandler->m_VertexList[vertexListIndex][i][j].getPosition());
                    particle.append(m_FileHandler->m_VertexList[vertexListIndex][i][j].getColor());
                    particle.append(m_FileHandler->m_VertexList[vertexListIndex][i][0].getPosition());
                }
                data.append(particle);
                numAtoms++;
            //}
        }
         qDebug() << "Polar View:: Data written";
        QOpenGLWidget::makeCurrent();
        m_VertexBuffer.destroy();
        m_VertexBuffer.create();
        m_VertexBuffer.bind();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        m_VertexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
        int bufferSize = 3*sizeof(QVector3D)*numAtoms*numFrames;
        int offset = numFrames * 3 * sizeof(QVector3D);
         qDebug() << "Polar View:: BufferSize: " << bufferSize << " numAtoms: " << numAtoms << " numFrames: " << numFrames;
        m_VertexBuffer.allocate(bufferSize);
         qDebug() << "Polar View:: Buffer Alocated";
         for (int i = 0; i < data.size(); i++) {
             m_VertexBuffer.write(i*offset, data[i].constData(), offset);
         }
         qDebug() << "Polar View:: Buffer Written";
         m_QueryIndex = queryIndex;
         m_DataIndex = vertexListIndex;
         m_hasData = true;
    } else if (queryIndex > 1 && vertexListIndex == m_DataIndex) {
        m_QueryIndex = queryIndex;
        m_DataIndex = vertexListIndex;
        m_hasData = true;
    }
    else {m_hasData = false;}
}

void PolarViewWindow::createGUIBuffer()
{
    qDebug() << "PolarViewWindow:: createGUIBuffer begin";
    QVector<QVector2D> lineVertices;
    QVector2D origin = QVector2D(this->width()/2.0,this->height()/2.0);
    lineVertices.append(origin);
    lineVertices.append(origin+QVector2D(2*(cos(m_MaxAngleFilter)),-2*(sin(m_MaxAngleFilter))));
    lineVertices.append(origin);
    lineVertices.append(origin+QVector2D(2*cos(m_MinAngleFilter),-2*sin(m_MinAngleFilter)));
    QOpenGLWidget::makeCurrent();
    m_GUIBuffer.create();
    m_GUIBuffer.bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_GUIBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_GUIBuffer.allocate(sizeof(QVector2D)* 4);
    m_GUIBuffer.write(0,lineVertices.constData(), sizeof(QVector2D) * 4);
    m_GUIBuffer.release();
    doneCurrent();
    qDebug() << "PolarViewWindow:: createGUIBuffer finish";
}

void PolarViewWindow::createCircleBuffer()
{

}

void PolarViewWindow::drawPaths()
{



    initializeOpenGLFunctions();
    makeCurrent();
    int NUMBER_OF_LIPID_ATOMS = m_FileHandler->m_VertexList[m_DataIndex].length();
    int frameOffset = m_SimLength*sizeof(Vertex);
    int offset = 3*sizeof(QVector3D);
    int TUPLE_SIZE = 3;
    m_PolarShdrPrgrm->bind();
    m_VertexBuffer.bind();
    if (m_isAAEnabled) glEnable(GL_LINE_SMOOTH);
    else glDisable(GL_LINE_SMOOTH);
    m_PolarShdrPrgrm->enableAttributeArray(0);
    m_PolarShdrPrgrm->enableAttributeArray(1);
    m_PolarShdrPrgrm->enableAttributeArray(2);

    m_PolarShdrPrgrm->setAttributeBuffer(0, GL_FLOAT,
                                  0,
                                  TUPLE_SIZE,
                                  offset);
    m_PolarShdrPrgrm->setAttributeBuffer(1, GL_FLOAT,
                                  sizeof(QVector3D),
                                  TUPLE_SIZE,
                                  offset);
    m_PolarShdrPrgrm->setAttributeBuffer(2, GL_FLOAT,
                                  2*sizeof(QVector3D),
                                  TUPLE_SIZE,
                                  offset);

    m_PolarShdrPrgrm->setUniformValue("modelToWorld",
                                   m_WorldMatrix);
    m_PolarShdrPrgrm->setUniformValue("worldToCamera",
                                   m_Camera->toMatrix());
    m_PolarShdrPrgrm->setUniformValue("cameraToView",
                                   m_Projection);
    m_PolarShdrPrgrm->setUniformValue("flatten",
                                   m_isFlatPerspective);
    glLineWidth(m_pathLineWidth);


    for (int i = 0; i < NUMBER_OF_LIPID_ATOMS; i++)
    {
        if (GlobalVars::g_DrawQueryList[m_QueryIndex].getBit(i)) { 
            double depth;
            if (m_DepthFilterType == 0) depth = m_FileHandler->m_VertexList[m_DataIndex][i][0].getPosition().z();
            else if (m_DepthFilterType == 1) depth = m_FileHandler->m_VertexList[m_DataIndex][i].back().getPosition().z();
            else {
                double start_depth = GlobalVars::g_FileHandler->m_VertexList[m_DataIndex][i].front().getPosition().z();
                double end_depth = GlobalVars::g_FileHandler->m_VertexList[m_DataIndex][i].back().getPosition().z();
                depth = start_depth - end_depth;
            }
            double displacement = GlobalVars::g_FileHandler->m_ParticleList[m_DataIndex][i].getParticleDisplacement();
            if (depth > m_MinDepthFilter && depth < m_MaxDepthFilter) {
                if (displacement > m_MinDisplacementFilter && displacement < m_MaxDisplacementFilter) {
                    m_PolarShdrPrgrm->setUniformValue("contextVal", 0);
                    glDrawArrays(GL_LINE_STRIP, i*51, 51);
                }
            }

        } else if (GlobalVars::g_DrawQueryList[m_QueryIndex].getPolarContextBit(i) && m_isContextDrawn) {
            m_PolarShdrPrgrm->setUniformValue("contextVal", 1);
            glDrawArrays(GL_LINE_STRIP, i*51, 51);
        }
    }



        m_PolarShdrPrgrm->release();
        m_VertexBuffer.release();
}

void PolarViewWindow::drawRedPaths()
{
    makeCurrent();
     int NUMBER_OF_LIPID_ATOMS = m_FileHandler->m_VertexList[1].length();
     int frameOffset = m_SimLength*sizeof(Vertex);
     int offset = sizeof(Vertex);
     int TUPLE_SIZE = 3;
     m_PolarShdrPrgrmScnd->bind();
     m_VertexBuffer.bind();

     m_PolarShdrPrgrmScnd->enableAttributeArray(0);
     m_PolarShdrPrgrmScnd->enableAttributeArray(1);

     m_PolarShdrPrgrmScnd->setAttributeBuffer(0, GL_FLOAT,
                                   Vertex::posOffset(),
                                   TUPLE_SIZE,
                                   offset);
     m_PolarShdrPrgrmScnd->setAttributeBuffer(1, GL_FLOAT,
                                   Vertex::colorOffset(),
                                   TUPLE_SIZE,
                                   offset);

     m_PolarShdrPrgrmScnd->setUniformValue("modelToWorld",
                                    m_WorldMatrix);
     m_PolarShdrPrgrmScnd->setUniformValue("worldToCamera",
                                    m_Camera->toMatrix());
     m_PolarShdrPrgrmScnd->setUniformValue("cameraToView",
                                    m_Projection);
     glLineWidth(0.1f);

     for (int i = 0; i < NUMBER_OF_LIPID_ATOMS; i=i+1)
     {
         //m_PolarShdrPrgrm->setUniformValue("startPoint", QVector3D(0,0,0));
         m_PolarShdrPrgrmScnd->setUniformValue("startPoint", QVector3D(m_FileHandler->m_VertexList[1][i][0].getPosition().x(),m_FileHandler->m_VertexList[1][i][0].getPosition().y(),m_FileHandler->m_VertexList[1][i][0].getPosition().z()));
         glDrawArrays(GL_LINE_STRIP, i*m_SimLength, m_SimLength);
     }

     m_PolarShdrPrgrmScnd->release();
     m_VertexBuffer.release();
}

void PolarViewWindow::drawGUI()
{
    const double GUI_DEPTH = 1.0;
    makeCurrent();

    double test = m_GUILineWidth/10.0;
     QVector3D points[48*3+1];
     QVector3D *points_ptr = points;
     int j = 0;
     for (int i = 0; i < 48; i++) {
         QVector3D point =  QVector3D(m_MaxPathLength*cos(float(i)*M_PI*2.0/48.0), m_MaxPathLength*sin(float(i)*M_PI*2.0/48.0),GUI_DEPTH);
         points[i+j] = QVector3D(point.x(),point.y(),GUI_DEPTH);
         if (i%12 == 0) {
             j++;
             QVector3D point2 =  QVector3D((m_MaxPathLength+0.4+0.4*test)*cos(float(i)*M_PI*2.0/48.0),
                                           (m_MaxPathLength+0.4+0.4*test)*sin(float(i)*M_PI*2.0/48.0),GUI_DEPTH);
             points[i+j] = QVector3D(point2.x(),point2.y(),GUI_DEPTH);
             j++;
             points[i+j] = QVector3D(point.x(),point.y(),GUI_DEPTH);
         } else if (i%12 == 6) {
             j++;
             QVector3D point2 =  QVector3D((m_MaxPathLength+0.3+0.3*test)*cos(float(i)*M_PI*2.0/48.0),
                                           (m_MaxPathLength+0.3+0.3*test)*sin(float(i)*M_PI*2.0/48.0),GUI_DEPTH);
             points[i+j] = QVector3D(point2.x(),point2.y(),GUI_DEPTH);
             j++;
             points[i+j] = QVector3D(point.x(),point.y(),GUI_DEPTH);
         } else if (i%6 == 2 || i%6 == 4) {
             j++;
             QVector3D point2 =  QVector3D((m_MaxPathLength+0.2+0.2*test)*cos(float(i)*M_PI*2.0/48.0),
                                           (m_MaxPathLength+0.2+0.2*test)*sin(float(i)*M_PI*2.0/48.0),GUI_DEPTH);
             points[i+j] = QVector3D(point2.x(),point2.y(),GUI_DEPTH);
             j++;
             points[i+j] = QVector3D(point.x(),point.y(),GUI_DEPTH);
         } else {
             j++;
             QVector3D point2 =  QVector3D((m_MaxPathLength+0.1+0.1*test)*cos(float(i)*M_PI*2.0/48.0),
                                           (m_MaxPathLength+0.1+0.1*test)*sin(float(i)*M_PI*2.0/48.0),GUI_DEPTH);
             points[i+j] = QVector3D(point2.x(),point2.y(),GUI_DEPTH);
             j++;
             points[i+j] = QVector3D(point.x(),point.y(),GUI_DEPTH);
         }
     }
     points[48*3] = QVector3D(m_MaxPathLength*cos(float(0.0)*M_PI*2.0/48.0), m_MaxPathLength*sin(float(0.0)*M_PI*2.0/48.0),GUI_DEPTH);

     m_LineShdrPrgm->bind();
     m_LineShdrPrgm->enableAttributeArray(0);
     m_LineShdrPrgm->setAttributeArray(0,points_ptr,0);
     m_LineShdrPrgm->setUniformValue("col",QVector4D(0.2, 0.2, 0.2, 0.9));
     m_LineShdrPrgm->setUniformValue("modelToWorld",
                                    m_WorldMatrix);
     m_LineShdrPrgm->setUniformValue("worldToCamera",
                                    m_Camera->toMatrix());
     m_LineShdrPrgm->setUniformValue("cameraToView",
                                    m_Projection);
     int count = 48*3+1;
     glLineWidth(m_GUILineWidth);

     glDrawArrays(GL_LINE_STRIP, 0, count);

     QVector3D origin =        QVector3D(0.0,                                        0.0,                                    GUI_DEPTH);
     QVector3D maxAnglePoint = QVector3D(1*m_MaxPathLength*cos(m_MaxAngleFilter),   1*m_MaxPathLength*sin(m_MaxAngleFilter), GUI_DEPTH);
     QVector3D minAnglePoint = QVector3D(1*m_MaxPathLength*cos(m_MinAngleFilter),   1*m_MaxPathLength*sin(m_MinAngleFilter), GUI_DEPTH);
     QVector3D angleLinePoints[4];
     points_ptr = angleLinePoints;
     angleLinePoints[0] = origin;
     angleLinePoints[1] = maxAnglePoint;
     angleLinePoints[2] = origin;
     angleLinePoints[3] = minAnglePoint;

     m_LineShdrPrgm->setAttributeArray(0,points_ptr,0);
     m_LineShdrPrgm->setUniformValue("col",QVector4D(0.2, 0.2, 0.2, 0.9));
     glDrawArrays(GL_LINE_STRIP, 0, 4);


     /* Max and min angle filter knobs */

     const int CIRCLE_RESOLUTION = 18;
     const float CIRCLE_RADIUS = 0.12f;
     QVector3D maxAngleLineKnobPoints[CIRCLE_RESOLUTION];
     points_ptr = maxAngleLineKnobPoints;
     for (int i = 0; i < CIRCLE_RESOLUTION; i++) {
         maxAngleLineKnobPoints[i] = maxAnglePoint + QVector3D(CIRCLE_RADIUS*cos(float(i)*M_PI*2.0/CIRCLE_RESOLUTION),
                                                               CIRCLE_RADIUS*sin(float(i)*M_PI*2.0/CIRCLE_RESOLUTION), GUI_DEPTH);
     }
     m_LineShdrPrgm->setAttributeArray(0,points_ptr,0);
     m_LineShdrPrgm->setUniformValue("col",QVector4D(0.8, 0.2, 0.2, 0.9));
     glDrawArrays(GL_TRIANGLE_FAN, 0, CIRCLE_RESOLUTION);

     QVector3D minAngleLineKnobPoints[CIRCLE_RESOLUTION];
     points_ptr = minAngleLineKnobPoints;
     for (int i = 0; i < CIRCLE_RESOLUTION; i++) {
         minAngleLineKnobPoints[i] = minAnglePoint + QVector3D(CIRCLE_RADIUS*cos(float(i)*M_PI*2.0/CIRCLE_RESOLUTION),
                                                               CIRCLE_RADIUS*sin(float(i)*M_PI*2.0/CIRCLE_RESOLUTION), GUI_DEPTH);
     }
     m_LineShdrPrgm->setAttributeArray(0,points_ptr,0);
     m_LineShdrPrgm->setUniformValue("col",QVector4D(0.2, 0.8, 0.2, 0.9));
     glDrawArrays(GL_TRIANGLE_FAN, 0, CIRCLE_RESOLUTION);

     /* The length filter cirlce */
     QVector3D filterCirclePoints[48];
     points_ptr = filterCirclePoints;
     for (int i = 0; i < 48; i++) {
         QVector3D point =  QVector3D(m_MaxPathLengthFilter*cos(float(i)*M_PI*2.0/48.0),
                                      m_MaxPathLengthFilter*sin(float(i)*M_PI*2.0/48.0), GUI_DEPTH);
         filterCirclePoints[i] = QVector3D(point.x(),point.y(),point.z());
     }
     m_LineShdrPrgm->setAttributeArray(0,points_ptr,0);
     m_LineShdrPrgm->setUniformValue("col",QVector4D(0.2, 0.2, 0.2, 0.8));
     glDrawArrays(GL_LINE_LOOP, 0, 48);

     for (int i = 0; i < 48; i++) {
         QVector3D point =  QVector3D((m_MaxPathLengthFilter-0.5)*cos(float(i)*M_PI*2.0/48.0),
                                      (m_MaxPathLengthFilter-0.5)*sin(float(i)*M_PI*2.0/48.0), GUI_DEPTH);
         filterCirclePoints[i] = QVector3D(point.x(),point.y(),point.z());
     }
     m_LineShdrPrgm->setAttributeArray(0,points_ptr,0);
     m_LineShdrPrgm->setUniformValue("col",QVector4D(0.2, 0.2, 0.2, 0.8));
     glDrawArrays(GL_LINE_LOOP, 0, 48);


     /******************************/

     /* Gray filled shape for filtered angle
     QVector3D filledCirclePoints[48];
     points_ptr = filledCirclePoints;
     filledCirclePoints[0] = origin;
     filledCirclePoints[1] = maxAnglePoint;
     int k = 0;
     for (int i = 0; i < 48; i++) {
         QVector3D point =  QVector3D(m_MaxPathLength*cos(float(i)*M_PI*2.0/48.0), m_MaxPathLength*sin(float(i)*M_PI*2.0/48.0),1.0);
         if (atan2(point.y(),point.x()) > atan2(minAnglePoint.y(), minAnglePoint.x()) &&
                 atan2(point.y(),point.x()) < atan2(maxAnglePoint.y(), maxAnglePoint.x())) {
             k = i;
         }
     }

     k++;
     for (int i = 2; i < 48; i++) {
         QVector3D point =  QVector3D(m_MaxPathLength*cos(float(k+i)*M_PI*2.0/48.0), m_MaxPathLength*sin(float(k+i)*M_PI*2.0/48.0),1.0);
         if (atan2(point.y(),point.x()) < atan2(minAnglePoint.y(), minAnglePoint.x()) ||
                 atan2(point.y(),point.x()) > atan2(maxAnglePoint.y(), maxAnglePoint.x())) {
             filledCirclePoints[i] = QVector3D(point.x(),point.y(),0.0);
         } else {
             filledCirclePoints[i] = minAnglePoint;
         }

     }

     m_LineShdrPrgm->setAttributeArray(0,points_ptr,0);
     m_LineShdrPrgm->setUniformValue("col",QVector4D(0.9, 0.9, 0.9, 0.3));
     glDrawArrays(GL_TRIANGLE_FAN, 0, 48);
     */


 //    for (int i = 0; i < 48; i++) {
 //        QVector3D point =  QVector3D(m_MaxPathLength*cos(float(i)*M_PI*2.0/48.0), m_MaxPathLength*sin(float(i)*M_PI*2.0/48.0),1.0);
 //        filledCirclePoints[i] = QVector3D(point.x(),point.y(),0.0);
 //    }

 //    m_LineShdrPrgm->setAttributeArray(0,points_ptr,0);
 //    m_LineShdrPrgm->setUniformValue("col",QVector4D(0.8, 0.8, 0.8, 0.1));
 //    glDrawArrays(GL_TRIANGLE_FAN, 0, 48);


     m_LineShdrPrgm->release();


 //    m_GUICircleShdrPrgm->bind();
 //    //glClearColor(.5, .5, .5, 1.0);
 //    //glClear(GL_COLOR_BUFFER_BIT);
 //    glEnable(GL_BLEND);
 //    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
 //
 //    glDisable(GL_CULL_FACE);
 //    m_GUICircleShdrPrgm->enableAttributeArray(0);
 //    m_GUICircleShdrPrgm->enableAttributeArray(1);
 //    m_GUICircleShdrPrgm->setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(float) * 5);
 //    m_GUICircleShdrPrgm->setAttributeBuffer(1, GL_FLOAT, sizeof(float) * 3, 2, sizeof(float) * 5);

 //    m_GUICircleShdrPrgm->setUniformValue("modelToWorld",
 //                                   m_WorldMatrix);
 //    m_GUICircleShdrPrgm->setUniformValue("worldToCamera",
 //                                   m_Camera->toMatrix());
 //    m_GUICircleShdrPrgm->setUniformValue("cameraToView",
 //                                   m_Projection);


 //        m_GUICircleShdrPrgm->setUniformValue("modelToWorld",
 //                                       m_GUIMatrix);
 //        m_GUICircleShdrPrgm->setUniformValue("cameraToView",
 //                                       m_Projection);
 //    glDrawArrays(GL_TRIANGLE_FAN, 0, 100);
 //    m_GUICircleShdrPrgm->release();
 //    m_CircleBuffer.release();
 //    int offset = sizeof(QVector2D);
 //    int TUPLE_SIZE = 2;
 //    m_PolarGUIShdrPrgrm->bind();
 //    m_GUIBuffer.bind();
 //    qDebug() << "GUI 2";
 //    m_PolarGUIShdrPrgrm->enableAttributeArray(0);
 //    m_PolarGUIShdrPrgrm->enableAttributeArray(1);
 //    m_PolarGUIShdrPrgrm->enableAttributeArray(2);
 //    m_PolarGUIShdrPrgrm->enableAttributeArray(3);

 //    m_PolarGUIShdrPrgrm->setAttributeBuffer(0, GL_FLOAT,
 //                                  0,
 //                                  TUPLE_SIZE,
 //                                  offset);
 //    m_PolarGUIShdrPrgrm->setAttributeBuffer(1, GL_FLOAT,
 //                                  sizeof(QVector2D),
 //                                  1,
 //                                  sizeof(float));
 //    m_PolarGUIShdrPrgrm->setAttributeBuffer(2, GL_FLOAT,
 //                                  sizeof(QVector2D) + sizeof(float),
 //                                  1,
 //                                  sizeof(float));
 //    m_PolarGUIShdrPrgrm->setAttributeBuffer(3, GL_FLOAT,
 //                                  sizeof(QVector2D) + 2 * sizeof(float),
 //                                  1,
 //                                  sizeof(float));
 //    qDebug() << "GUI 3";
 //    m_PolarGUIShdrPrgrm->setUniformValue("radius",
 //                                   radius);
 //    m_PolarGUIShdrPrgrm->setUniformValue("minAngle",
 //                                         float(m_MinAngleFilter+M_PI));
 //    m_PolarGUIShdrPrgrm->setUniformValue("maxAngle",
 //                                   float(m_MaxAngleFilter+M_PI));
 //    m_PolarGUIShdrPrgrm->setUniformValue("modelToWorld",
 //                                   m_GUIMatrix);
 //    m_PolarGUIShdrPrgrm->setUniformValue("cameraToView",
 //                                   m_Projection);

 //    glLineWidth(1.0f);
 //    glDrawArrays(GL_POINTS,0, 5);
 //    qDebug() << "GUI 4";
 //    //glDrawArrays(GL_LINE_STRIP, 0, 2);
 //    //glDrawArrays(GL_LINE_STRIP, 2, 2);
 //    guiBuffer.release();
 //    //m_GUIBuffer.release();
 //    m_PolarGUIShdrPrgrm->release();
     //    qDebug() << "GUI 5";
}

void PolarViewWindow::drawGrid()
{
    makeCurrent();
    QVector3D points[8];
    QVector3D *points_ptr = points;

    points[0] = QVector3D(0.0,  0.0,    15.0);
    points[1] = QVector3D(0.0,  10.0,   15.0);
    points[2] = QVector3D(0.0,  0.0,    15.0);
    points[3] = QVector3D(10.0, 0.0,    15.0);
    points[4] = QVector3D(0.0,  0.0,    15.0);
    points[5] = QVector3D(0.0,  -10.0,  15.0);
    points[6] = QVector3D(0.0,  0.0,    15.0);
    points[7] = QVector3D(-10.0,0.0,    15.0);

    m_LineShdrPrgm->bind();
    m_LineShdrPrgm->enableAttributeArray(0);
    m_LineShdrPrgm->setAttributeArray(0,points_ptr,0);
    m_LineShdrPrgm->setUniformValue("col",QVector4D(0.2, 0.2, 0.2, 0.8));
    m_LineShdrPrgm->setUniformValue("modelToWorld",
                                   m_WorldMatrix);
    m_LineShdrPrgm->setUniformValue("worldToCamera",
                                   m_Camera->toMatrix());
    m_LineShdrPrgm->setUniformValue("cameraToView",
                                   m_Projection);
    int count = 8;
    glLineWidth(1.0f);

    glDrawArrays(GL_LINE_STRIP, 0, count);
    m_LineShdrPrgm->release();
}

void PolarViewWindow::changeFilter()
{

}

void PolarViewWindow::keyReleaseEvent(QKeyEvent *e)
{

}

void PolarViewWindow::mousePressEvent(QMouseEvent *event)
{
    double MOUSE_ANGLE_MARGIN = 0.1;
    m_MouseMoving = true;
    if (event->button() == Qt::LeftButton) {
        QVector2D maxAnglePoint = QVector2D(m_MaxPathLength*cos(m_MaxAngleFilter),   m_MaxPathLength*sin(m_MaxAngleFilter));
        QVector2D minAnglePoint = QVector2D(m_MaxPathLength*cos(m_MinAngleFilter),   m_MaxPathLength*sin(m_MinAngleFilter));
        QVector2D mouseCoordinate = QVector2D(event->pos().x()-(width()/2.0),        -1*(event->pos().y()-(height()/2.0)));

        double minAngleATan = atan2(minAnglePoint.y(), minAnglePoint.x());
        double maxAngleATan = atan2(maxAnglePoint.y(), maxAnglePoint.x());
        double mouseATan = atan2(mouseCoordinate.y(), mouseCoordinate.x());

        qDebug() << "PolarViewWindow::mousePressEvent maxAnglePoint: " << maxAnglePoint << " minAnglePoint: " << minAnglePoint << "mouse coord: " << mouseCoordinate;
        if ((mouseATan+minAngleATan)/2.0 >= minAngleATan-MOUSE_ANGLE_MARGIN && (mouseATan+minAngleATan)/2.0 <= minAngleATan+MOUSE_ANGLE_MARGIN) {
            m_MinAngleChanging = true;
            m_AngleDiff =  m_MinAngleFilter - m_MaxAngleFilter;
        } else if ((mouseATan+maxAngleATan)/2.0 >= maxAngleATan-MOUSE_ANGLE_MARGIN && (mouseATan+maxAngleATan)/2.0 <= maxAngleATan+MOUSE_ANGLE_MARGIN) {
            m_MaxAngleChanging = true;
            m_AngleDiff = m_MaxAngleFilter - m_MinAngleFilter;
        } else if (event->modifiers() & Qt::ControlModifier){
            m_LengthFilterChanging = true;
        }
    } else if (event->button() == Qt::RightButton) {
        m_RightMouse = true;
    }
}

void PolarViewWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (m_MouseMoving) {
        double windowCentreX = this->width()/2.0;
        double windowCentreY = this->height()/2.0;
        QVector2D mouseCoordinate = QVector2D(event->pos().x()-(width()/2.0),        -1*(event->pos().y()-(height()/2.0)));

        if (m_MaxAngleChanging || m_MinAngleChanging) {
            if (m_isLockAngle) {
                if(m_MaxAngleChanging) {
                    m_MaxAngleFilter = atan2(mouseCoordinate.y(), mouseCoordinate.x());
                    m_MinAngleFilter = m_MaxAngleFilter - m_AngleDiff;
                    m_MinAngleFilter = atan2(sin(m_MinAngleFilter), cos(m_MinAngleFilter));
                } else {
                    m_MinAngleFilter = atan2(mouseCoordinate.y(), mouseCoordinate.x());
                    m_MaxAngleFilter = m_MinAngleFilter - m_AngleDiff;
                    m_MaxAngleFilter = atan2(sin(m_MaxAngleFilter), cos(m_MaxAngleFilter));
                }
                emit angleFilterChanged(m_MinAngleFilter, m_MaxAngleFilter);
                m_SomethingChanged = true;
            } else if (m_MaxAngleChanging) {
                m_MaxAngleFilter = atan2(mouseCoordinate.y(), mouseCoordinate.x());
                emit angleFilterChanged(m_MinAngleFilter, m_MaxAngleFilter);
                m_SomethingChanged = true;
            } else if (m_MinAngleChanging) {
                m_MinAngleFilter = atan2(mouseCoordinate.y(), mouseCoordinate.x());
                emit angleFilterChanged(m_MinAngleFilter, m_MaxAngleFilter);
                m_SomethingChanged = true;
            }
        } else if (m_LengthFilterChanging) {
            m_MaxPathLengthFilter = MathFuncs::normalize(0.0,sqrt(pow(double(width()/2.0),2)),0.0, m_MaxPathLength,sqrt(pow(event->x()-double(width()/2.0),2) + pow(event->y()-double(height()/2.0),2)));
            m_MaxDepthLengthFilter = MathFuncs::normalize(0.0,sqrt(pow(double(width()/2.0),2)),0.0, 10.0,sqrt(pow(event->x()-double(width()/2.0),2) + pow(event->y()-double(height()/2.0),2)));

            m_SomethingChanged = true;
            qDebug() << "Max:" << m_MaxPathLengthFilter << m_MaxDepthLengthFilter;
        }
        update();
    }
}

void PolarViewWindow::mouseReleaseEvent(QMouseEvent *event)
{
    qDebug() << "PolarViewWindow:: mouse Release Event";
    if (m_SomethingChanged && m_hasData) {
        double windowCentreX = this->width()/2.0;
        double windowCentreY = this->height()/2.0;
        QVector2D mouseCoordinate = QVector2D(event->pos().x()-(width()/2.0),        -1*(event->pos().y()-(height()/2.0)));
        double mouse_atan = atan2(mouseCoordinate.y(), mouseCoordinate.x());

        if (m_MaxAngleChanging || m_MinAngleChanging) {
            if (m_isLockAngle) {
                if(m_MaxAngleChanging) {
                    m_MaxAngleFilter = mouse_atan;
                    m_MinAngleFilter = m_MaxAngleFilter - m_AngleDiff;
                    m_MinAngleFilter = atan2(sin(m_MinAngleFilter), cos(m_MinAngleFilter));
                } else {
                    m_MinAngleFilter = mouse_atan;
                    m_MaxAngleFilter = m_MinAngleFilter - m_AngleDiff;
                    m_MaxAngleFilter = atan2(sin(m_MaxAngleFilter), cos(m_MaxAngleFilter));
                }
            } else if (m_MaxAngleChanging) m_MaxAngleFilter = mouse_atan;
            else if (m_MinAngleChanging) m_MinAngleFilter = mouse_atan;
        }
//        if (m_MinAngleFilter > m_MaxAngleFilter) {
//            double temp = m_MinAngleFilter;
//            m_MinAngleFilter = m_MaxAngleFilter;
//            m_MaxAngleFilter = temp;
//        }

        qDebug() << "PolarViewWindow::mouseReleaseEvent mouseCoordinate: " << mouseCoordinate
                 << "m_MaxAngleFilter:" << qRadiansToDegrees(m_MaxAngleFilter)
                 << "m_minAngleFilter:" << qRadiansToDegrees(m_MinAngleFilter)
                 << "mouse atan:" << qRadiansToDegrees(mouse_atan);

        emit angleFilterChanged(m_MinAngleFilter, m_MaxAngleFilter);
        m_ColorMap = GlobalVars::g_ColorMapList[0];
        for (int i = 0; i < GlobalVars::g_DrawQueryList[m_QueryIndex].getBitArray().size(); i++) {
            double startX = m_FileHandler->m_VertexList[m_DataIndex][i][0].getPosition().x();
            double startY = m_FileHandler->m_VertexList[m_DataIndex][i][0].getPosition().y();
            double endX = m_FileHandler->m_VertexList[m_DataIndex][i][50].getPosition().x();
            double endY = m_FileHandler->m_VertexList[m_DataIndex][i][50].getPosition().y();
            double startZ = m_FileHandler->m_VertexList[m_DataIndex][i][0].getPosition().z();
            double endZ = m_FileHandler->m_VertexList[m_DataIndex][i][50].getPosition().z();
            double theta = atan2(endY - startY, endX - startX);
            double distance = sqrt(pow(endX-startX,2) + pow(endY-startY,2));
            double depth = qAbs(endZ-startZ);
            GlobalVars::g_DrawQueryList[m_QueryIndex].setColorMapMinVal(-M_PI);
            GlobalVars::g_DrawQueryList[m_QueryIndex].setColorMapMaxVal(M_PI);
            QColor col;

            bool isFiltered = true;
            if (m_MinAngleFilter < m_MaxAngleFilter) {
                isFiltered = (theta <= m_MinAngleFilter) || (theta >= m_MaxAngleFilter) ||
                        (m_isMaxLengthFilter && distance > m_MaxPathLengthFilter) ||
                        (!m_isMaxLengthFilter && distance < m_MaxPathLengthFilter);
            } else {
                isFiltered = ((theta <= m_MinAngleFilter) && (theta >= m_MaxAngleFilter)) ||
                        (m_isMaxLengthFilter && distance > m_MaxPathLengthFilter) ||
                        (!m_isMaxLengthFilter && distance < m_MaxPathLengthFilter);
            }
            GlobalVars::g_DrawQueryList[m_QueryIndex].setPolarBit(i, !isFiltered);
        }
        emit dataChanged(m_DataIndex);
    }
    m_SomethingChanged = false;
    m_MouseMoving = false;
    m_LeftMouse = false;
    m_RightMouse = false;
    m_MinAngleChanging = false;
    m_MaxAngleChanging = false;
    m_LengthFilterChanging = false;

    update();

}

void PolarViewWindow::wheelEvent(QWheelEvent *event)
{
    int SCROLL_SPEED = 5;
    int direction = 1;
    if (event->delta() > 0) direction = -1;
    m_Camera->translate(m_Camera->forward()*direction*SCROLL_SPEED*0.1);
    update();
}

void PolarViewWindow::calcMaxPathLength()
{
    int NUMBER_OF_LIPID_ATOMS = m_FileHandler->m_VertexList[1].length();
    m_MaxPathLength = 0;
    for (int i = 0; i < NUMBER_OF_LIPID_ATOMS; i=i+1)
    {
        double startX = m_FileHandler->m_VertexList[1][i][0].getPosition().x();
        double startY = m_FileHandler->m_VertexList[1][i][0].getPosition().y();
        double endX = m_FileHandler->m_VertexList[1][i][50].getPosition().x();
        double endY = m_FileHandler->m_VertexList[1][i][50].getPosition().y();
        double distance = sqrt(pow(endX-startX,2) + pow(endY-startY,2));
        if (distance < 10 && distance > m_MaxPathLength) m_MaxPathLength = distance;
    }
    m_MaxPathLengthFilter = m_MaxPathLength;
}

void PolarViewWindow::updateColors()
{
    DrawQuery *curQuery = &GlobalVars::g_DrawQueryList[m_QueryIndex];
    switch(curQuery->getColorMappingVar()){
      case 0:
        for (int i = 0; i < GlobalVars::g_DrawQueryList[m_QueryIndex].getBitArray().size(); i++) {

        }
        break;

      case 1:

        break;

      case 2:

        break;

      default:

        break;
    }
}
