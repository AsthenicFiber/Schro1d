#-------------------------------------------------
#
# Project created by QtCreator 2016-08-25T08:59:32
#
#-------------------------------------------------

QT       += core gui
QT += widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Schro1d
TEMPLATE = app

FORMS += \
    mainwindow.ui

HEADERS += \
    mainwindow.h \
    lapacke.h \
    lapacke_config.h \
    lapacke_mangling.h \
    matrix.h \
    schro_pois.h \
    mesh.h \
    materialsdata.h \
    highlighter.h \
    graphicsplot.h

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    matrix.cpp \
    schro_pois.cpp \
    mesh.cpp \
    materialsdata.cpp \
    highlighter.cpp \
    graphicsplot.cpp

unix:!macx: CONFIG += qt debug

win32: LIBS += -L$$PWD/../ -llibblas

INCLUDEPATH += $$PWD/../
DEPENDPATH += $$PWD/../

win32: LIBS += -L$$PWD/../ -lliblapack

INCLUDEPATH += $$PWD/../
DEPENDPATH += $$PWD/../

win32: LIBS += -L$$PWD/../ -lliblapacke

INCLUDEPATH += $$PWD/../
DEPENDPATH += $$PWD/../

unix:!macx: LIBS += -L$$PWD/./ -lblas

unix:!macx: LIBS += -L$$PWD/./ -llapack

unix:!macx: LIBS += -L$$PWD/./ -llapacke

DISTFILES += \
    materials.csv \
    test4.txt
