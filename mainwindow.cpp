#include "mainwindow.h"
#include "IO/filehandler.h"
#include "euclideanviewwindow.h"
#include "polarviewwindow.h"
#include "Data/drawquery.h"
#include "Data/vertex.h"
#include "Data/globalvars.h"
#include "Data/particle.h"

#include "GUI/timelinewidget.h"
#include "GUI/querydrawsetupwidget.h"
#include "GUI/optionswidget.h"

#include <QSplitter>
#include <QDebug>
#include <QTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    GlobalVars::init();
    m_FileHandler = new FileHandler;
    GlobalVars::g_FileHandler = m_FileHandler;
    createActions();
    createMenus();
    createSubWidgets();


    /******************************** REFACTOR BELOW ********************************/
    QTime time;
    bool test;
    time.start();

    QString filename = "C:/Users/yahoo/OneDrive/Desktop/MDS Data/popepopg51/protstruct.txt";
    test = m_FileHandler->readStructFile(filename);
    filename = "C:/Users/yahoo/OneDrive/Desktop/MDS Data/popepopg51/protdata.bin";
    m_FileHandler->readBinFile(filename);

    qDebug() << "time: " << time.elapsed();
    time.restart();

    filename = "C:/Users/yahoo/OneDrive/Desktop/MDS Data/popepopg51/lipidstruct.txt";
    test = m_FileHandler->readStructFile(filename);
    filename = "C:/Users/yahoo/OneDrive/Desktop/MDS Data/popepopg51/lipiddata.bin";
    m_FileHandler->readBinFile(filename);

    qDebug() << "time: " << time.elapsed();
    time.restart();

    qDebug() << "MainWindow:: m_FileHandler->getSimBox(): " << m_FileHandler->getSimBox();
    m_TimelineWidget->setNumberOfFrames(m_FileHandler->m_VertexList[0][0].length());

    GlobalVars::g_SimulationBox = m_FileHandler->getSimBox();

    /******************************** REFACTOR ABOVE ********************************/

    /******************************** REFACTOR BELOW ********************************/

    DrawQuery testQuery;
    testQuery.setBufferIndex(1);
    testQuery.setParentQueryIndex(1);
    testQuery.setDrawPath(true);
    testQuery.setDrawParticle(false);
    testQuery.setQueryName("lipiddata.bin");
    testQuery.setQueryType(0);
    qDebug() << "MainWindow:: testQuery buffer index: " << testQuery.getDataBufferIndex();
    testQuery.setBitArray(QBitArray(m_FileHandler->m_VertexList[1].length()));
    int numpbc = 0;
    for (int i = 0; i < m_FileHandler->m_VertexList[1].length(); i++) {
        double dist = 0;
        bool bit = true;
        for (int j = 0; j < 50; ++j) {
            QVector2D p1(m_FileHandler->m_VertexList[1][i][j].getPosition());
            QVector2D p2(m_FileHandler->m_VertexList[1][i][j+1].getPosition());
            dist += p1.distanceToPoint(p2);
            if (dist > 10) {
                numpbc++;
                bit = false;
                break;
            }
        }
        m_FileHandler->m_ParticleList[1][i].setParticleDisplacement(dist);
        testQuery.setBit(i,bit);
    }
    qDebug() << "MainWindow:: PBC crossings: " << numpbc;


    DrawQuery testQuery2;
    testQuery2.setBufferIndex(0);
    testQuery2.setParentQueryIndex(0);
    testQuery2.setDrawPath(false);
    testQuery2.setDrawParticle(true);
    testQuery2.setContext(0);
    testQuery2.setQueryName("protdata.bin");
    testQuery2.setQueryType(0);
    testQuery2.setBitArray(QBitArray(m_FileHandler->m_VertexList[0].length()));
    for (int i = 0; i < m_FileHandler->m_VertexList[0].length(); i++) {
        double dist = 0;
        bool bit = true;
        for (int j = 0; j < 50; ++j) {
            QVector2D p1(m_FileHandler->m_VertexList[1][i][j].getPosition());
            QVector2D p2(m_FileHandler->m_VertexList[1][i][j+1].getPosition());
            dist += p1.distanceToPoint(p2);
            if (dist > 10) {
                bit = false;
                break;
            }
        }
        m_FileHandler->m_ParticleList[0][i].setParticleDisplacement(dist);
        testQuery2.setBit(i,bit);
    }
    qDebug() << "MainWindow::GlobalVars::g_DataFileList.length(): " << GlobalVars::g_DataFileList.length();
    qDebug() << "MainWindow::GlobalVars::g_DataFileList[0].id: " << GlobalVars::g_DataFileList[0].id;
    qDebug() << "MainWindow::GlobalVars::g_DataFileList[0].filename: " << GlobalVars::g_DataFileList[0].filename;
    qDebug() << "MainWindow::GlobalVars::g_DataFileList[1].id: " << GlobalVars::g_DataFileList[1].id;
    qDebug() << "MainWindow::GlobalVars::g_DataFileList[1].filename: " << GlobalVars::g_DataFileList[1].filename;
    GlobalVars::g_DrawQueryList.append(testQuery2);
    qDebug() << "MainWindow::testQuery.getDataBufferIndex(): " << testQuery.getDataBufferIndex();
    GlobalVars::g_DrawQueryList.append(testQuery);
    qDebug() << "MainWindow:::GlobalVars::g_DrawQueryList.first().getDataBufferIndex(): " << GlobalVars::g_DrawQueryList.first().getDataBufferIndex();
    m_QueryDrawSetupWidget->addQuery(0);
    m_QueryDrawSetupWidget->addQuery(1);
    /******************************** REFACTOR ABOVE ********************************/
}

MainWindow::~MainWindow()
{

}

void MainWindow::euclideanFrameRendered()
{
    m_EuclidFPS++;
}

void MainWindow::polarFrameRendered()
{
    m_PolarFPS++;
}

void MainWindow::updateFPS()
{
    //qDebug() << "EuclideanView FPS: " << m_EuclidFPS << "PolarView FPS: " << m_PolarFPS;
    m_EuclidFPS = 0;
    m_PolarFPS = 0;
}

void MainWindow::keyReleaseEvent(QKeyEvent *e)
{
    if(e->key() == Qt::Key_W)
    {
        emit CameraKeyPressed("W");
    }
    if(e->key() == Qt::Key_A)
    {
        emit CameraKeyPressed("A");
    }
    if(e->key() == Qt::Key_S)
    {
        emit CameraKeyPressed("S");
    }
    if(e->key() == Qt::Key_D)
    {
        emit CameraKeyPressed("D");
    }
    if(e->key() == Qt::Key_Up)
    {
        emit CameraKeyPressed("Up");
    }
    if(e->key() == Qt::Key_Down)
    {
        emit CameraKeyPressed("Down");
    }
    if(e->key() == Qt::Key_L)
    {
        emit CameraKeyPressed("l");
    }
    if(e->key() == Qt::Key_Control)
    {
        emit ModifierPressed(e);
    }
}

void MainWindow::createMenus()
{
    m_FileMenu = menuBar()->addMenu("&File");
    m_FileMenu->addAction(m_FileLoadMolDataAction);
    m_FileMenu->addAction(m_FileLoadStateAction);
    m_FileMenu->addAction(m_FileSaveStateAction);
    m_FileMenu->addAction(m_FileExportAction);
    m_FileMenu->addAction(m_FileClearSessionAction);
    m_FileMenu->addAction(m_FileQuitAction);


    m_GraphicsMenu = menuBar()->addMenu("&Graphics");
    m_DisplayMenu = menuBar()->addMenu("&Display");
    m_HelpMenu = menuBar()->addMenu("&Help");
}

void MainWindow::createActions()
{
    m_FileLoadMolDataAction = new QAction("Load Molecular Data");
    m_FileLoadStateAction = new QAction("Load work state");
    m_FileSaveStateAction = new QAction("Save work state");
    m_FileExportAction = new QAction("Export");
    m_FileClearSessionAction = new QAction("Clear Session");
    m_FileQuitAction = new QAction("Quit");
}

void MainWindow::createSubWidgets()
{
    m_TabWidget = new QTabWidget;
    m_TimelineWidget = new TimelineWidget;
    m_OptionsWidget = new OptionsWidget;
    m_EuclideanViewWindow = new EuclideanViewWindow;
    m_PolarViewWindow = new PolarViewWindow;
    m_QueryDrawSetupWidget = new QueryDrawSetupWidget;





    m_EuclideanViewWindow->testfh = m_FileHandler;
    connect(this, SIGNAL(CameraKeyPressed(QString)),
            m_EuclideanViewWindow, SLOT(CameraKeyController(QString)));
    connect(this, SIGNAL(CameraKeyPressed(QString)), m_PolarViewWindow, SLOT(CameraKeyController(QString)));
    connect(m_TimelineWidget, SIGNAL(frameChanged(int)), m_EuclideanViewWindow, SLOT(setFrameNumber(int)));

    connect(m_QueryDrawSetupWidget, SIGNAL(colorChanged(int)), m_EuclideanViewWindow, SLOT(rewriteVertexBuffer(int)));
    connect(m_PolarViewWindow, SIGNAL(dataChanged(int)), m_EuclideanViewWindow, SLOT(rewriteVertexBuffer(int)));
    connect(m_QueryDrawSetupWidget, SIGNAL(newPolarQuery(int, int)), m_PolarViewWindow, SLOT(rewriteVertexBuffer(int, int)));
    connect(m_QueryDrawSetupWidget, SIGNAL(colorChanged(int)), m_PolarViewWindow, SLOT(colorChanged(int)));
    connect(m_QueryDrawSetupWidget, SIGNAL(queryChanged()), m_PolarViewWindow, SLOT(queryChanged()));
    connect(m_QueryDrawSetupWidget, SIGNAL(somethingChanged()), m_EuclideanViewWindow, SLOT(update()));
    connect(m_QueryDrawSetupWidget, SIGNAL(depthFilterChanged(int, int)), m_EuclideanViewWindow, SLOT(changeDepthFilter(int, int)));
    connect(m_QueryDrawSetupWidget, SIGNAL(depthFilterChanged(int, int)), m_PolarViewWindow, SLOT(changeDepthFilter(int, int)));
    connect(m_QueryDrawSetupWidget, SIGNAL(depthFilterTypeChanged(int)), m_EuclideanViewWindow, SLOT(changeDepthFilterType(int)));
    connect(m_QueryDrawSetupWidget, SIGNAL(depthFilterTypeChanged(int)), m_PolarViewWindow, SLOT(changeDepthFilterType(int)));
    connect(m_QueryDrawSetupWidget, SIGNAL(displacementFilterChanged(int, int)), m_EuclideanViewWindow, SLOT(changeDisplacementFilter(int, int)));
    connect(m_QueryDrawSetupWidget, SIGNAL(displacementFilterChanged(int, int)), m_PolarViewWindow, SLOT(changeDisplacementFilter(int, int)));


    m_PolarViewWindow->setFileHandler(m_FileHandler);

    QSplitter *mainWidget = new QSplitter;
    QSplitter *topSplitter = new QSplitter(Qt::Horizontal);
    QHBoxLayout *topLayout = new QHBoxLayout;
    topLayout->addWidget(m_EuclideanViewWindow);
    topLayout->addWidget(m_PolarViewWindow);
    QWidget *botWidget = new QWidget;
    QVBoxLayout *botLayout = new QVBoxLayout;
    botLayout->addLayout(topLayout);
    botLayout->addWidget(m_TimelineWidget);
    botLayout->addWidget(m_TabWidget);
    botWidget->setLayout(botLayout);

    m_TabWidget->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);


    QWidget *topWidget = new QWidget;
    //topWidget->setLayout(topLayout);

    //mainWidget->addWidget(topWidget);
    //mainWidget->addWidget(botWidget);
    mainWidget->setOrientation(Qt::Vertical);
    mainWidget->setStretchFactor(1,0);
    mainWidget->setStretchFactor(0,1);

//    QVBoxLayout *mainLayout = new QVBoxLayout;
//    mainWidget->setLayout(mainLayout);

    //m_TabWidget->addTab(m_TimelineWidget, "Playback");
    m_TabWidget->addTab(m_QueryDrawSetupWidget, "Queries");
    m_TabWidget->addTab(m_OptionsWidget, "Settings");

//    mainLayout->addWidget(m_EuclideanViewWindow);
//    mainLayout->addWidget(m_TabWidget);

    connect(m_OptionsWidget, SIGNAL(resetCameraPressed()),
            m_EuclideanViewWindow, SLOT(resetCamera()));
    connect(m_OptionsWidget, SIGNAL(backgroundColorChanged(QColor)),
            m_EuclideanViewWindow, SLOT(changeBackgroundColor(QColor)));
    connect(m_OptionsWidget, SIGNAL(euclideanAAChanged(int)),
            m_EuclideanViewWindow, SLOT(changeAA(int)));

    connect(m_OptionsWidget, SIGNAL(backgroundColorChanged(QColor)),
            m_PolarViewWindow, SLOT(changeBackgroundColor(QColor)));
    connect(m_OptionsWidget, SIGNAL(polarGUISizeChanged(int)),
            m_PolarViewWindow, SLOT(changeGUISize(int)));
    connect(m_OptionsWidget, SIGNAL(polarTrajectorySizeChanged(int)),
            m_PolarViewWindow, SLOT(changeTrajectorySize(int)));
    connect(m_OptionsWidget, SIGNAL(drawGUIChanged(int)),
            m_PolarViewWindow, SLOT(setGUIVisible(int)));
    connect(m_OptionsWidget, SIGNAL(drawGridChanged(int)),
            m_PolarViewWindow, SLOT(setGridVisible(int)));
    connect(m_OptionsWidget, SIGNAL(drawContextChanged(int)),
            m_PolarViewWindow, SLOT(setContextVisible(int)));
    connect(m_OptionsWidget, SIGNAL(lockAngleChanged(int)),
            m_PolarViewWindow, SLOT(setLockAngle(int)));
    connect(m_OptionsWidget, SIGNAL(lengthFilterTypeChanged(bool)),
            m_PolarViewWindow, SLOT(setLengthFilterType(bool)));


    connect(m_OptionsWidget, SIGNAL(polarAAChanged(int)),
            m_PolarViewWindow, SLOT(changeAA(int)));
    connect(m_OptionsWidget, SIGNAL(flatPerspectiveChanged(int)),
            m_PolarViewWindow, SLOT(changeFlatPerspective(int)));



    connect(m_PolarViewWindow, SIGNAL(angleFilterChanged(double, double)),
            m_OptionsWidget, SLOT(changeAngleFilters(double, double)));

    m_FPSTimer = new QTimer(this);
    m_FPSTimer->setInterval(1000);
    m_FPSTimer->start();

    connect(m_PolarViewWindow, SIGNAL(frameRendered()),
            this, SLOT(polarFrameRendered()));
    connect(m_EuclideanViewWindow, SIGNAL(frameRendered()),
            this, SLOT(euclideanFrameRendered()));
    connect(m_FPSTimer,SIGNAL(timeout()),
            this, SLOT(updateFPS()));

    setCentralWidget(botWidget);
}
