#ifndef QUICHECONSOLE_H
#define QUICHECONSOLE_H

#include <QtQml/qqml.h>
#include <QObject>

class QuicheConsole : public QObject
{
    Q_OBJECT
public:
    explicit QuicheConsole(QObject *parent = 0);
    
    Q_INVOKABLE void sendRequest(const QByteArray&);
    
};

QML_DECLARE_TYPE(QuicheConsole)

#endif // QUICHECONSOLE_H
