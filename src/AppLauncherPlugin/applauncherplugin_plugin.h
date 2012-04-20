#ifndef APPLAUNCHERPLUGIN_PLUGIN_H
#define APPLAUNCHERPLUGIN_PLUGIN_H

#include <QtQml/QQmlExtensionPlugin>

class AppLauncherPluginPlugin : public QDeclarativeExtensionPlugin
{
    Q_OBJECT
    
public:
    void registerTypes(const char *uri);
};

#endif // APPLAUNCHERPLUGIN_PLUGIN_H

