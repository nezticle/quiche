TEMPLATE = lib
TARGET = AppLauncherPlugin
TARGETPATH = Quiche
QT += qml
CONFIG += qt plugin

TARGET = $$qtLibraryTarget($$TARGET)
uri = Quiche

target.path = /usr/imports/$$TARGETPATH

qmldir.files += $$PWD/qmldir
qmldir.path +=  /usr/imports/$$TARGETPATH

# Input
SOURCES += \
    applauncherplugin_plugin.cpp \
    applaunchermodel.cpp \
    quicheconsole.cpp

HEADERS += \
    applauncherplugin_plugin.h \
    applaunchermodel.h \
    quicheconsole.h

OTHER_FILES = qmldir \
    AppLauncherPluginPlugin.json

INSTALLS += target qmldir
