#include "applauncherplugin_plugin.h"
#include "applaunchermodel.h"
#include "quicheconsole.h"

#include <QtQml/qqml.h>

void AppLauncherPluginPlugin::registerTypes(const char *uri)
{
    // @uri Quiche
    qmlRegisterType<AppLauncherModel>(uri, 1, 0, "AppLauncherModel");
    qmlRegisterType<QuicheConsole>(uri, 1, 0, "QuicheConsole");
}

