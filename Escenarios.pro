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
    utils.cpp \
    gui/results/customfiledialognewlayout.cpp \
    gui/results/resultsprocessui.cpp \
    gui/scenarios/materialconfigui.cpp \
    gui/scenarios/materialescenariosgeneralconfig.cpp \
    gui/scenarios/materialgeneralconfig.cpp \
    gui/scenarios/scenariomaterialsconfigui.cpp \
    gui/scenarios/scenariosconfigui.cpp \
    gui/scenarios/scenariosismicconfigui.cpp \
    model/results/resultfile.cpp \
    model/results/resultmethod.cpp \
    model/scenarios/escenariofile.cpp \
    model/scenarios/escenariomaterialcustom.cpp \
    model/scenarios/escenarioseismiccustom.cpp \
    model/scenarios/material.cpp \
    model/scenarios/materialproperty.cpp

HEADERS  += mainwindow.h \
    gui/results/customfiledialognewlayout.h \
    gui/results/resultsprocessui.h \
    gui/scenarios/materialconfigui.h \
    gui/scenarios/materialescenariosgeneralconfig.h \
    gui/scenarios/materialgeneralconfig.h \
    gui/scenarios/scenariomaterialsconfigui.h \
    gui/scenarios/scenariosconfigui.h \
    gui/scenarios/scenariosismicconfigui.h \
    model/results/resultfile.h \
    model/results/resultmethod.h \
    model/scenarios/escenariofile.h \
    model/scenarios/escenariomaterialcustom.h \
    model/scenarios/escenarioseismiccustom.h \
    model/scenarios/material.h \
    model/scenarios/materialproperty.h \
    utils.h

FORMS    += mainwindow.ui \
    gui/results/customfiledialognewlayout.ui \
    gui/results/resultsprocessui.ui \
    gui/scenarios/materialconfigui.ui \
    gui/scenarios/materialescenariosgeneralconfig.ui \
    gui/scenarios/materialgeneralconfig.ui \
    gui/scenarios/scenariomaterialsconfigui.ui \
    gui/scenarios/scenariosconfigui.ui \
    gui/scenarios/scenariosismicconfigui.ui
