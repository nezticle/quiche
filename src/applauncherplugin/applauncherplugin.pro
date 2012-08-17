TEMPLATE = lib
TARGET = AppLauncherPlugin
TARGETPATH = Quiche
QT += qml
CONFIG += qt plugin

TARGET = $$qtLibraryTarget($$TARGET)
uri = Quiche

qmldir.files += $$PWD/qmldir

linux-rasp-pi-g++{
    target.path = /usr/imports/$$TARGETPATH
    qmldir.path +=  /usr/imports/$$TARGETPATH
} else {
    target.path = $$[QT_INSTALL_IMPORTS]/$$TARGETPATH
    qmldir.path = $$[QT_INSTALL_IMPORTS]/$$TARGETPATH
}

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
