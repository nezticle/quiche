#include "quicheconsole.h"
#include <stdio.h>


QuicheConsole::QuicheConsole(QObject *parent) :
    QObject(parent)
{
}

void QuicheConsole::sendRequest(const QByteArray &request)
{
    printf("%s\n", request.constData());
}

void QuicheConsole::launchApplication(const QByteArray &target)
{
    printf("QUEUE %s\n", target.constData());
}
