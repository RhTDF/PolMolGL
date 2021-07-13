#include "mainwindow.h"
#include <QApplication>
#include "ColorMap/colormapwidget.h"



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

//    ColorMapWidget *widget = new ColorMapWidget;
//    widget->show();
    MainWindow *mainWindow = new MainWindow;
    mainWindow->move(50,50);
    mainWindow->show();

    //QueryTreeWindow *mw = new QueryTreeWindow;
    //mw->show();

    return a.exec();
}
