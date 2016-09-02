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

#LIBS += -L$$PWD/./ -lblas_win64_MT

#INCLUDEPATH += $$PWD/.
#DEPENDPATH += $$PWD/.

#LIBS += -L$$PWD/./ -llapack_win64_MT

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.

LIBS += \
    -lblas_win64_MT \
    -llapack_win64_MT
