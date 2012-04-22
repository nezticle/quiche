TEMPLATE = lib
TARGET = AppLauncherPlugin
TARGETPATH = Quiche
QT += declarative
CONFIG += qt plugin

TARGET = $$qtLibraryTarget($$TARGET)
uri = Quiche

target.path = $$[QT_INSTALL_IMPORTS]/$$TARGETPATH

qmldir.files += $$PWD/qmldir
qmldir.path +=  $$[QT_INSTALL_IMPORTS]/$$TARGETPATH

# Input
SOURCES += \
    applauncherplugin_plugin.cpp \
    applaunchermodel.cpp \
    quicheconsole.cpp

HEADERS += \
    applauncherplugin_plugin.h \
    applaunchermodel.h \
    quicheconsole.h

OTHER_FILES = qmldir

INSTALLS += target qmldir
