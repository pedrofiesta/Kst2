include($$PWD/../../../../kst.pri)

QT += xml qt3support

TEMPLATE = lib
CONFIG += plugin
OBJECTS_DIR = tmp
MOC_DIR = tmp
TARGET = kstplugin_syncbin
DESTDIR = $$OUTPUT_DIR/plugin

INCLUDEPATH += \
    tmp \
    $$TOPLEVELDIR/src/libkst \
    $$TOPLEVELDIR/src/libkstmath \
    $$TOPLEVELDIR/src/widgets \
    $$OUTPUT_DIR/src/widgets \
    $$OUTPUT_DIR/src/datasources/ascii/tmp

LIBS += -lkst -lkstmath -lkstwidgets -lkstapp

SOURCES += \
    syncbin.cpp

HEADERS += \
    syncbin.h

FORMS += syncbinconfig.ui