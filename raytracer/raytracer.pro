QT += core
QT -= gui
#QT -= qt

#CONFIG += c++11
#LIBRARIES = -lm
TARGET = rayTracer
CONFIG += console
CONFIG -= app_bundle
#QMAKE_CXXFLAGS_DEBUG += -pg -O3
#QMAKE_LFLAGS_DEBUG += -pg -O3

INCLUDEPATH += C:\Users\etien\Documents\projets\eigen\
    C:\Users\SauvageE.ANTYCIPSIMU\Documents\GitHub\eigen\
    C:\Users\etien\Documents\projets\json\single_include\
    C:\Users\SauvageE.ANTYCIPSIMU\Documents\GitHub\json\single_include\
    C:\Users\etien\Documents\projets\CImg\
    C:\Users\SauvageE.ANTYCIPSIMU\Documents\GitHub\CImg\
    ../common

TEMPLATE = app

SOURCES += \
	main.cpp \
	raytracer.cpp

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

HEADERS += \
	raytracer.h
DEPENDPATH += ../common

win32: LIBS += -lGdi32

LIBS += -L../common -lcommon
