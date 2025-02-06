QT += core
QT += gui widgets concurrent

#CONFIG += c++11
#LIBRARIES = -lm
TARGET = rayTracer
CONFIG += console
CONFIG -= app_bundle
#QMAKE_CXXFLAGS_DEBUG += -pg -O3
#QMAKE_LFLAGS_DEBUG += -pg -O3

INCLUDEPATH += C:\Users\etien\Documents\projets\eigen\
    C:\Users\sauvagee\Documents\GitHub\eigen\
    C:\Users\etien\Documents\projets\json\single_include\
    "D:\Sauvegarde Mes Documents\GitHub\json\single_include"\
    C:\Users\etien\Documents\projets\CImg\
    "D:\Sauvegarde Mes Documents\GitHub\CImg"\
  #  ../common

TEMPLATE = app

SOURCES += \
	../common/aabb.cpp \
	../common/boule.cpp \
	../common/bvhnode.cpp \
	../common/camera.cpp \
	../common/composedmaterial.cpp \
	../common/cuboid.cpp \
	../common/dielectrique.cpp \
	../common/horizon.cpp \
	../common/image.cpp \
	../common/lambertien.cpp \
	../common/material.cpp \
	../common/mesh.cpp \
	../common/meshnode.cpp \
	../common/metal.cpp \
	../common/parameter.cpp \
	../common/phongbliss.cpp \
	../common/primitive_factory.cpp \
	../common/scene.cpp \
	../common/shapelist.cpp \
	../common/sun.cpp \
	../common/texture.cpp \
	../common/triangle.cpp \
	camerav.cpp \
	main.cpp \
	meshv.cpp \
        # modeler.cpp \
	raytracer.cpp \
        # renderer.cpp \
        # shader.cpp \
        # vulkanwindow.cpp

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
	../common/aabb.h \
	../common/boule.h \
	../common/bvhnode.h \
	../common/camera.h \
	../common/composedmaterial.h \
	../common/cuboid.h \
	../common/dielectrique.h \
	../common/horizon.h \
	../common/image.h \
	../common/lambertien.h \
	../common/material.h \
	../common/mesh.h \
	../common/meshnode.h \
	../common/metal.h \
	../common/parameter.h \
	../common/phongbliss.h \
	../common/primitive_factory.h \
	../common/rayon.h \
	../common/scene.h \
	../common/shape.h \
	../common/shapelist.h \
	../common/sun.h \
	../common/texture.h \
	../common/triangle.h \
	../common/utils.h \
	camerav.h \
	meshv.h \
        # modeler.h \
	raytracer.h \
        # renderer.h \
        # shader.h \
        # vulkanwindow.h
#DEPENDPATH += ../common

win32: LIBS += -lGdi32

#LIBS += -L../common -lcommon

FORMS += \
    modeler.ui

RESOURCES += \
    raytracer.qrc

#SUBDIRS += \
#    ../common/common.pro
