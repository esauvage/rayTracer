TEMPLATE = lib
CONFIG = staticlib

INCLUDEPATH += C:\Users\etien\Documents\projets\eigen\
    C:\Users\SauvageE.ANTYCIPSIMU\Documents\GitHub\eigen\
    C:\Users\etien\Documents\projets\json\single_include\
    C:\Users\SauvageE.ANTYCIPSIMU\Documents\GitHub\json\single_include\
    C:\Users\etien\Documents\projets\CImg\
    C:\Users\SauvageE.ANTYCIPSIMU\Documents\GitHub\CImg\

SOURCES += \
	aabb.cpp \
	bvhnode.cpp \
	camera.cpp \
	dielectrique.cpp \
	image.cpp \
	lambertien.cpp \
	boule.cpp \
	horizon.cpp \
	material.cpp \
	mesh.cpp \
	metal.cpp \
        parameter.cpp \
        primitive_factory.cpp \
	scene.cpp \
	shapelist.cpp \
	texture.cpp \
	triangle.cpp
HEADERS += \
	aabb.h \
	boule.h \
	bvhnode.h \
	camera.h \
	dielectrique.h \
	horizon.h \
	image.h \
	lambertien.h \
	material.h \
	mesh.h \
	metal.h \
        parameter.h \
        primitive_factory.h \
	rayon.h \
	scene.h \
	shape.h\
	shapelist.h \
	texture.h \
	triangle.h \
	utils.h