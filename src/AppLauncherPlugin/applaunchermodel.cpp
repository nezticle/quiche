#include "applaunchermodel.h"
#include <QDebug>

AppLauncherModel::AppLauncherModel(QObject *parent)
    : QAbstractListModel(parent)
{
    //This sets the role names that will be accesible
    //in the delegate code in QML
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    roles[DescriptionRole] = "description";
    roles[TypeRole] = "type";
    roles[ThumbnailRole] = "thumbnail";
    roles[WorkingDirectoryRole] = "workingDirectory";
    roles[TargetRole] = "target";
    roles[ArgumentsRole] = "args";

    setRoleNames(roles);

    connect(this, SIGNAL(applicationDirectoryChanged()), this, SLOT(updateModelData()));
}

AppLauncherModel::~AppLauncherModel()
{
    foreach(ApplicationMetaData *dataItem, m_applicationDataList)
        delete dataItem;
}

void AppLauncherModel::setApplicationDirectory(const QString &directoryPath)
{
    m_applicationDirectory.setPath(directoryPath);
    emit applicationDirectoryChanged();
}

int AppLauncherModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_applicationDataList.count();
}

QVariant AppLauncherModel::data(const QModelIndex &index, int role) const
{
    //Check for valid index
    if ( index.row() < 0 || index.row() > m_applicationDataList.count())
        return QVariant();

    const ApplicationMetaData *appData = m_applicationDataList[index.row()];

    switch ( role ) {

    case NameRole:
        return appData->name;
    case DescriptionRole:
        return appData->description;
    case TypeRole:
        return appData->type;
    case ThumbnailRole:
        return appData->thumbnail;
    case WorkingDirectoryRole:
        return appData->workingDirectory;
    case TargetRole:
        return appData->target;
    case ArgumentsRole:
        return appData->args;
    default:
        //invalid role requested
        return QVariant();
    }
}

void AppLauncherModel::updateModelData()
{
    //Search for all directories in the Applications Folder:
    searchDirectoryForApplication(m_applicationDirectory);
}

void AppLauncherModel::parseMetaDataFile(const QString &metaDataFile, AppLauncherModel::ApplicationMetaData *appData)
{
    Q_UNUSED(metaDataFile)
    Q_UNUSED(appData)
    //TODO: Parse Meta Data file when found

    //Name

    //Description

    //Thumbnail/Screenshot

    //Arguments
}

void AppLauncherModel::searchDirectoryForApplication(const QDir &dir)
{
    qDebug() << "searchDirectoryForApplication: " << dir;

    QStringList directories = dir.entryList(QDir::AllDirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);

    foreach ( QString directory, directories)
    {
        QDir currentDirectory(QString(dir.absolutePath() + "/" + directory));

        //Search for binary target first
        QFileInfoList filesInDirectory = currentDirectory.entryInfoList(QDir::Executable);

        qDebug() << directory << " has " << filesInDirectory.count() << "apps";

        foreach (QFileInfo binary, filesInDirectory)
        {
            ApplicationMetaData *currentApp = new ApplicationMetaData;

            currentApp->target = binary.fileName();
            currentApp->workingDirectory = binary.filePath();
            currentApp->type = QString("native");
            currentApp->name = currentApp->target; //Default name is target

            //check for meta-data
            QString metaDataFile = QString(currentApp->target + ".meta");
            if(currentDirectory.exists(metaDataFile))
                parseMetaDataFile(QString(binary.filePath() + "/" + metaDataFile), currentApp);

            m_applicationDataList.append(currentApp);
        }

        //TODO: check for QML runtime applications, should probably need meta data attached.
//        QStringList qmlFilter;
//        qmlFilter << "*.qml";

        //Check sub directories.
        searchDirectoryForApplication(currentDirectory);
    }
}



