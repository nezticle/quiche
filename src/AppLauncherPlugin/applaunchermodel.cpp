#include "applaunchermodel.h"

#include <QtCore/QSettings>

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
    roles[IconRole] = "icon";
    roles[PathRole] = "path";
    roles[TargetRole] = "target";
    roles[ArgumentsRole] = "args";

    setRoleNames(roles);

    connect(this, SIGNAL(applicationDirectoryChanged()), this, SLOT(updateModelData()));
}

AppLauncherModel::~AppLauncherModel()
{
    foreach (ApplicationMetaData *dataItem, m_applicationDataList)
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
    case IconRole:
        return appData->icon;
    case PathRole:
        return appData->path;
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
    searchDirectoryForApplications(m_applicationDirectory);
}

void AppLauncherModel::setIconPath(const QString &searchPath)
{
  m_iconPath = searchPath.split(";");
}

QString AppLauncherModel::iconPath() const
{
  return m_iconPath.join(";");
}


static QString lookupIcon(const QString &name, const QString &dir)
{
  qDebug() << "lookupIcon" << name << dir;

  QString pathName = dir + "/" + name; 
  if (QFileInfo(pathName).isFile())
    return pathName;
  if (QFileInfo(pathName + ".png").isFile())
    return pathName + ".png";
  if (QFileInfo(pathName + ".jpg").isFile())
    return pathName + ".jpg";
  if (QFileInfo(pathName + ".xpm").isFile())
    return pathName + ".xpm";
  if (QFileInfo(pathName + ".bmp").isFile())
    return pathName + ".bmp";
  //yes, we could do this in better ways....

  return QString();
}

QString AppLauncherModel::findIcon(const QString &name, const QString &dir) const
{
  qDebug() << "findIcon" << name << dir;

  if (name.isEmpty())
    return lookupIcon(QLatin1String("icon"), dir);

  QString res = lookupIcon(name, dir);
  if (!res.isEmpty())
    return res;
  foreach (QString searchDir, m_iconPath) {
    res = lookupIcon(name, searchDir);
  if (!res.isEmpty())
    return res;
  }
  return QString();
}


bool AppLauncherModel::parseMetaDataFile(const QDir &directory, const QString &metaDataFile, AppLauncherModel::ApplicationMetaData *appData)
{
    QString metaFileLocation(directory.absolutePath() + "/" + metaDataFile);

    //.desktop files are in IniFormat, so we can parse them easily using QSettings
    QSettings applicationMetaDataFile(metaFileLocation, QSettings::IniFormat);

    applicationMetaDataFile.beginGroup("Quiche");
    appData->name = applicationMetaDataFile.value("Name", directory.dirName()).toString();
    appData->description = applicationMetaDataFile.value("Description", "").toString();
    appData->type = applicationMetaDataFile.value("Type", "").toString();
    appData->target = applicationMetaDataFile.value("Target", "").toString();
    appData->path = applicationMetaDataFile.value("Path", directory.absolutePath()).toString();
    appData->icon = findIcon(applicationMetaDataFile.value("Icon", "").toString(), appData->path);
    appData->args = applicationMetaDataFile.value("Arguments", "").toString();

    //If there is no target, then this isn't a valid .desktop file
    if (appData->target.isEmpty())
        return false;

    if (appData->type.isEmpty()) {
        if (appData->target.endsWith(".qml"))
            appData->type = "QML";
        else
            appData->type = "Application";
    }

    return true;

}

void AppLauncherModel::searchDirectoryForApplications(const QDir &dir)
{
    //Create a List of Application Subdirectories
    QStringList directories = dir.entryList(QDir::AllDirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);

    foreach ( QString directory, directories) {
        QDir currentDirectory(QString(dir.absolutePath() + "/" + directory));

        //First check for *.desktop files (which contain application meta data)
        QStringList filters;
        filters << "*.desktop";

        QStringList desktopFiles = currentDirectory.entryList(filters);

        foreach ( QString desktopFile, desktopFiles) {
            ApplicationMetaData *currentApp = new ApplicationMetaData;

            //If the meta file is valid, add it to the model data list.
            if (parseMetaDataFile(currentDirectory, desktopFile, currentApp))
                m_applicationDataList.append(currentApp);
            else
                delete currentApp;
        }

        //If there was at least one .desktop file, move the next folder
        if (desktopFiles.count() > 0)
            continue;

        //At this point, since there was no meta data given, try to determine it on context
        ApplicationMetaData *contextBasedApplication = applicationFromContext(currentDirectory);
        if(contextBasedApplication)
            m_applicationDataList.append(contextBasedApplication);
    }
}

AppLauncherModel::ApplicationMetaData *AppLauncherModel::applicationFromContext(const QDir &dir)
{
    ApplicationMetaData *currentApp = new ApplicationMetaData;

    //Applications will reside in a Directory of the same name
    currentApp->name = dir.dirName();
    currentApp->path = dir.absolutePath();

    //Search for a executable target
    QFileInfoList executableList = dir.entryInfoList(QDir::Files | QDir::Executable);

    //If there is one executable, then this is the target
    if (executableList.count() == 1) {
        currentApp->target = executableList.at(0).fileName();
        currentApp->type = QString("application");
    }
    else if ( executableList.count() > 1) {
        //Skip this folder, as there is no right answer.
        delete currentApp;
        return 0;
    } else {
        //There were no excutables, so lets try QML files now
        QStringList filters;
        filters << "*.qml";
        QFileInfoList qmlFileList = dir.entryInfoList(filters);
        currentApp->type = QString("qml");

        //If there is one qml file, this is the target
        if (qmlFileList.count() == 1) {
            currentApp->target = qmlFileList.at(0).fileName();
        } else if (qmlFileList.isEmpty()) {
            //There is no hope now, move on to the next folder
            delete currentApp;
            return 0;
        } else {
            //If there are more than one qml file, but there is one file that starts with a lowercase letter (eg main.qml)
            int targetCount = 0;
            QString target;
            foreach (QFileInfo qmlFile, qmlFileList) {
                QString filename = qmlFile.fileName();
                char firstLetter = filename[0].toAscii(); //BUG: so, so, very hackey...

                if ( firstLetter >= 'a' && firstLetter <= 'z' ) {
                    target = filename;
                    targetCount++;
                }
            }
            //If there was more than one qml file meeting the criteria, that was a waste of time
            if (targetCount != 1) {
                delete currentApp;
                return 0;
            }
            currentApp->target = target;
        }
    }
    //Now that we have the bare minimum of a Name, Path, and Target currentApp is valid

    //Make an attempt to find an icon
    currentApp->icon = findIcon(QString(""), dir.path());

    return currentApp;

}



