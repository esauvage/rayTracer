TEMPLATE = subdirs
SUBDIRS = common raytracer stl2json

common.subdir  = common
raytracer.subdir = raytracer
stl2json.subdir  = stl2json

raytracer.depends = common
stl2json.depends = common
