QT += core
QT -= gui
#QT -= qt

#CONFIG += c++11
#LIBRARIES = -lm
TARGET = stl2json
CONFIG += console
CONFIG -= app_bundle
#QMAKE_CXXFLAGS_DEBUG += -pg -O3
#QMAKE_LFLAGS_DEBUG += -pg -O3

INCLUDEPATH += C:\Users\etien\Documents\projets\eigen\
    C:\Users\sauvagee\Documents\GitHub\eigen\
    C:\Users\etien\Documents\projets\json\single_include\
    "D:\Sauvegarde Mes Documents\GitHub\json\single_include"\
    C:\Users\etien\Documents\projets\stl_reader\
    "D:\Sauvegarde Mes Documents\GitHub\stl_reader"\
    ../common

TEMPLATE = app

SOURCES += \
        main.cpp \
        obj2json.cpp \
        stl2json.cpp

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
        obj2json.h \
        objload.h \
        stl2json.h

win32: LIBS += -lGdi32

LIBS += -L../common -lcommon
