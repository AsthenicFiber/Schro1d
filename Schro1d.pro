#-------------------------------------------------
#
# Project created by QtCreator 2016-08-25T08:59:32
#
#-------------------------------------------------

QT       += core gui

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
    matrix.h

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    matrix.cpp

unix:!macx: CONFIG += qt debug

#LIBS += -L$$PWD/./ -lblas_win64_MT

#INCLUDEPATH += $$PWD/.
#DEPENDPATH += $$PWD/.

#LIBS += -L$$PWD/./ -llapack_win64_MT

#INCLUDEPATH += $$PWD/.
#DEPENDPATH += $$PWD/.

#LIBS += -L$$PWD/./ -llibblas
#LIBS += -L$$PWD/./ -lliblapack
#LIBS += -L$$PWD/./ -lliblapacke


win32: LIBS += -L$$PWD/../ -llibblas

INCLUDEPATH += $$PWD/../
DEPENDPATH += $$PWD/../

win32: LIBS += -L$$PWD/../ -lliblapack

INCLUDEPATH += $$PWD/../
DEPENDPATH += $$PWD/../

win32: LIBS += -L$$PWD/../ -lliblapacke

INCLUDEPATH += $$PWD/../
DEPENDPATH += $$PWD/../

#win32: LIBS += -L$$PWD/./ -llibblas

#INCLUDEPATH += $$PWD/.
#DEPENDPATH += $$PWD/.

#win32:!win32-g++: PRE_TARGETDEPS += $$PWD/./libblas.lib
#else:win32-g++: PRE_TARGETDEPS += $$PWD/./liblibblas.a

#win32: LIBS += -L$$PWD/./ -lliblapack

#INCLUDEPATH += $$PWD/.
#DEPENDPATH += $$PWD/.

#win32:!win32-g++: PRE_TARGETDEPS += $$PWD/./liblapack.lib
#else:win32-g++: PRE_TARGETDEPS += $$PWD/./libliblapack.a

#win32: LIBS += -L$$PWD/./ -lliblapacke

#INCLUDEPATH += $$PWD/.
#DEPENDPATH += $$PWD/.

#win32:!win32-g++: PRE_TARGETDEPS += $$PWD/./liblapacke.lib
#else:win32-g++: PRE_TARGETDEPS += $$PWD/./libliblapacke.a

unix:!macx: LIBS += -L$$PWD/./ -lblas

#INCLUDEPATH += $$PWD/.
#DEPENDPATH += $$PWD/.

unix:!macx: LIBS += -L$$PWD/./ -llapack

#INCLUDEPATH += $$PWD/.
#DEPENDPATH += $$PWD/.

unix:!macx: LIBS += -L$$PWD/./ -llapacke

#INCLUDEPATH += $$PWD/.
#DEPENDPATH += $$PWD/.
