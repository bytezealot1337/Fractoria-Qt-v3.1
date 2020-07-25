#-------------------------------------------------
#
# Project created by QtCreator 2014-08-26T17:44:44
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Fractoria
TEMPLATE = app

win32:RC_ICONS += tangramIcon.ico

SOURCES += main.cpp\
        fractoria.cpp \
    renderthread.cpp \
    newtonrootcoloring.cpp \
    coloring.cpp \
    animationProperties.cpp \
    animationthread.cpp \
    animationviewer.cpp \
    options.cpp \
    ifs.cpp

HEADERS  += fractoria.h \
    renderthread.h \
    newtonrootcoloring.h \
    coloring.h \
    animationProperties.h \
    animationthread.h \
    animationviewer.h \
    options.h \
    ifs.h

FORMS    += fractoria.ui \
    newtonrootcoloring.ui \
    coloring.ui \
    animationProperties.ui \
    animationviewer.ui \
    options.ui \
    ifs.ui
