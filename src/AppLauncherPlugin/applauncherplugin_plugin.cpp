#include "applauncherplugin_plugin.h"
#include "applaunchermodel.h"

#include <QtQml/qqml.h>

void AppLauncherPluginPlugin::registerTypes(const char *uri)
{
    // @uri Quiche
    qmlRegisterType<AppLauncherModel>(uri, 1, 0, "AppLauncherModel");
}

Q_EXPORT_PLUGIN2(AppLauncherPlugin, AppLauncherPluginPlugin)

