#-------------------------------------------------
#
# Project created by QtCreator 2014-11-15T15:28:12
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Escenarios
TEMPLATE = app


QMAKE_CXXFLAGS += -std=c++0x -U__STRICT_ANSI__

SOURCES += main.cpp\
        mainwindow.cpp \
    escenariofile.cpp \
    utils.cpp \
    material.cpp \
    materialproperty.cpp \
    scenariomaterialsconfigui.cpp \
    materialconfigui.cpp \
    scenariosconfigui.cpp \
    scenariosismicconfigui.cpp \
    resultsprocessui.cpp \
    resultfile.cpp

HEADERS  += mainwindow.h \
    escenariofile.h \
    utils.h \
    material.h \
    materialproperty.h \
    scenariomaterialsconfigui.h \
    materialconfigui.h \
    scenariosconfigui.h \
    scenariosismicconfigui.h \
    resultsprocessui.h \
    resultfile.h

FORMS    += mainwindow.ui \
    scenariomaterialsconfigui.ui \
    materialconfigui.ui \
    scenariosconfigui.ui \
    scenariosismicconfigui.ui \
    resultsprocessui.ui
