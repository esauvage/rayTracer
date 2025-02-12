QT += gui

TEMPLATE = lib
DEFINES += TESTCOMMON_LIBRARY

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += staticlib

INCLUDEPATH += C:\Users\etien\Documents\projets\eigen\
    C:\Users\sauvagee\Documents\GitHub\eigen\
    C:\Users\etien\Documents\projets\json\single_include\
    "D:\Sauvegarde Mes Documents\GitHub\json\single_include"\
    C:\Users\etien\Documents\projets\CImg\
    "D:\Sauvegarde Mes Documents\GitHub\CImg"\

SOURCES += \
	aabb.cpp \
        bvhnode.cpp \
        composedmaterial.cpp \
        cuboid.cpp \
	dielectrique.cpp \
	image.cpp \
        lambertien.cpp \
        light.cpp \
        boule.cpp \
	horizon.cpp \
	material.cpp \
        mesh.cpp \
        meshnode.cpp \
	metal.cpp \
        parameter.cpp \
        phongbliss.cpp \
        primitive_factory.cpp \
	scene.cpp \
        shapelist.cpp \
        sun.cpp \
	texture.cpp \
	triangle.cpp
HEADERS += \
	aabb.h \
	boule.h \
        bvhnode.h \
        composedmaterial.h \
        cuboid.h \
	dielectrique.h \
	horizon.h \
	image.h \
        lambertien.h \
        light.h \
        material.h \
        mesh.h \
        meshnode.h \
	metal.h \
        parameter.h \
        phongbliss.h \
        primitive_factory.h \
	rayon.h \
	scene.h \
	shape.h\
        shapelist.h \
        sun.h \
	texture.h \
	triangle.h \
	utils.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
