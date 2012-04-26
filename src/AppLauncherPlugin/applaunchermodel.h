#ifndef APPLAUNCHERMODEL_H
#define APPLAUNCHERMODEL_H

#include <QtQml/qqml.h>
#include <QAbstractListModel>
#include <QDir>

class AppLauncherModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(QString applicationDirectory READ applicationDirectory WRITE setApplicationDirectory NOTIFY applicationDirectoryChanged)
    Q_PROPERTY(QString iconPath READ iconPath WRITE setIconPath)

    struct ApplicationMetaData {
        QString name;
        QString description;
        QString type;
        QString icon;
        QString path;
        QString target;
        QString args;
    };

public:
    enum TilesetRoles {
        NameRole = Qt::UserRole + 1,
        DescriptionRole,
        TypeRole,
        IconRole,
        PathRole,
        TargetRole,
        ArgumentsRole
    };

    explicit AppLauncherModel( QObject *parent = 0);
    ~AppLauncherModel();

    void setApplicationDirectory(const QString &directoryPath);
    const QString applicationDirectory() { return m_applicationDirectory.absolutePath(); }

    void setIconPath(const QString &searchPath);
    QString iconPath() const;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role) const;

signals:
    void applicationDirectoryChanged();

private slots:
    void updateModelData();

private:
    Q_DISABLE_COPY(AppLauncherModel)

    bool parseMetaDataFile(const QDir &directory, const QString &metaDataFile, ApplicationMetaData *appData);
    QString findIcon(const QString &name, const QString &dir) const;
    void searchDirectoryForApplications(const QDir &dir);
    ApplicationMetaData* applicationFromContext(const QDir &dir);

    QList<ApplicationMetaData*> m_applicationDataList;
    QDir m_applicationDirectory;
    QStringList m_iconPath;
};

QML_DECLARE_TYPE(AppLauncherModel)

#endif // APPLAUNCHERMODEL_H

