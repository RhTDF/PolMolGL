#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QVBoxLayout>
#include <QTabWidget>
#include<QMenuBar>
#include<QMenu>

class TimelineWidget;
class QueryDrawSetupWidget;
class FileHandler;
class EuclideanViewWindow;
class PolarViewWindow;
class OptionsWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
public slots:
    void euclideanFrameRendered();
    void polarFrameRendered();
    void updateFPS();
signals:
    void CameraKeyPressed(QString);
    void ModifierPressed(QKeyEvent *);
protected:
    void keyReleaseEvent(QKeyEvent *e) override;

private:
    void createMenus();
    void createActions();
    void createSubWidgets();

    FileHandler *m_FileHandler;
    TimelineWidget *m_TimelineWidget;
    QueryDrawSetupWidget *m_QueryDrawSetupWidget;
    OptionsWidget *m_OptionsWidget;
    EuclideanViewWindow *m_EuclideanViewWindow;
    PolarViewWindow *m_PolarViewWindow;
    QTabWidget *m_TabWidget;


    QMenu *m_FileMenu;
    QAction *m_FileLoadMolDataAction;
    QAction *m_FileLoadStateAction;
    QAction *m_FileSaveStateAction;
    QAction *m_FileExportAction;
    QAction *m_FileClearSessionAction;
    QAction *m_FileQuitAction;

    QMenu *m_GraphicsMenu;
    QAction *m_GraphicsColorMapAction;

    QMenu *m_DisplayMenu;

    QMenu *m_HelpMenu;

    int m_PolarFPS = 0;
    int m_EuclidFPS = 0;
    QTimer *m_FPSTimer;
};

#endif // MAINWINDOW_H
