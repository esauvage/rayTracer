TEMPLATE = subdirs
SUBDIRS = common raytracer stl2json
raytracer.depends = common
stl2json.depends = common
