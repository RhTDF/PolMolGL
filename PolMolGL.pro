#-------------------------------------------------
#
# Project created by QtCreator 2019-08-29T19:23:23
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PolMolGL
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11
LIBS   += -lopengl32

SOURCES += \
        Data/globalvars.cpp \
        Data/particle.cpp \
        Data/vertex.cpp \
        IO/filehandler.cpp \
        main.cpp \
        mainwindow.cpp \
    euclideanviewwindow.cpp \
    euclideanviewcamera.cpp \
    Data/drawquery.cpp \
    GUI/timelinewidget.cpp \
    GUI/querydrawsetupwidget.cpp \
    GUI/attributeviewwindow.cpp \
    polarviewwindow.cpp \
    mathfuncs.cpp \
    ColorMap/colormap.cpp \
    ColorMap/colormapcategorical.cpp \
    ColorMap/colormapdivergent.cpp \
    ColorMap/colormaprainbow.cpp \
    ColorMap/colormapsequential.cpp \
    ColorMap/colormapwidget.cpp \
    ColorMap/newcolormapwidget.cpp \
    Utility/rangeslider.cpp \
    GUI/optionswidget.cpp

HEADERS += \
        Data/globalvars.h \
        Data/particle.h \
        Data/vertex.h \
        IO/filehandler.h \
        mainwindow.h \
    euclideanviewwindow.h \
    euclideanviewcamera.h \
    Data/drawquery.h \
    GUI/timelinewidget.h \
    GUI/querydrawsetupwidget.h \
    GUI/attributeviewwindow.h \
    polarviewwindow.h \
    mathfuncs.h \
    ColorMap/colormap.h \
    ColorMap/colormapcategorical.h \
    ColorMap/colormapdivergent.h \
    ColorMap/colormaprainbow.h \
    ColorMap/colormapsequential.h \
    ColorMap/colormapwidget.h \
    ColorMap/newcolormapwidget.h \
    Utility/rangeslider.h \
    GUI/optionswidget.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    shaders.qrc \
    icons.qrc
