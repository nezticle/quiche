#ifndef APPLAUNCHERPLUGIN_PLUGIN_H
#define APPLAUNCHERPLUGIN_PLUGIN_H

#include <QtQml/QQmlExtensionPlugin>

class AppLauncherPluginPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface" FILE "AppLauncherPluginPlugin.json")
    
public:
    void registerTypes(const char *uri);
};

#endif // APPLAUNCHERPLUGIN_PLUGIN_H

