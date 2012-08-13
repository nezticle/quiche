#include "processmanager.h"
#include "process.h"

#include <qtimer.h>
#include <qdebug.h>
#include <qdir.h>

PiProcessManager::PiProcessManager(int argc, char**argv)
  :QCoreApplication(argc, argv)
  , m_currentProcess(0), m_terminateNextProcessDelay(-1)
{
  QTimer::singleShot(0, this, SLOT(launchNextProcess()));
  m_nextDirectory = m_currentDirectory = QDir::currentPath().toLatin1();

  //TODO use QString for file names
}

PiProcessManager::~PiProcessManager()
{
  delete m_currentProcess;
}

void PiProcessManager::queue(const QByteArray &commandLine)
{
  qDebug() << "queueing" << commandLine << "with dir" << m_nextDirectory;
  m_commandStack.append(PiCommand(commandLine, m_nextDirectory));
  m_nextDirectory = m_currentDirectory;
}

void PiProcessManager::requeueCurrent()
{
  qDebug() << "requeueing" << m_currentCommand.commandLine;
  m_commandStack.append(m_currentCommand);
}

void PiProcessManager::setDirectory(const QByteArray &dir)
{
  m_nextDirectory =  QDir(m_currentDirectory).absoluteFilePath(dir).toLatin1();
  qDebug() << "setDirectory" << m_nextDirectory;
}

//slots:

void PiProcessManager::terminateCurrentProcess()
{
  qDebug() << "PiProcessManager::terminateCurrentProcess";
  if (m_currentProcess)
    m_currentProcess->terminateProcess();
}

void PiProcessManager::terminateNextProcessAfter(int msec)
{
  qDebug() << "PiProcessManager::terminateNextProcessAfter" << msec;
  m_terminateNextProcessDelay = msec;
}

void PiProcessManager::timerEvent(QTimerEvent * event)
{
  if (event->timerId() == m_terminationTimer.timerId()) {
    qDebug() << "PiProcessManager::timerEvent() terminating" << m_currentCommand.commandLine;
    terminateCurrentProcess();
  }
}



void PiProcessManager::launchNextProcess()
{
  qDebug() << "PiProcessManager::launchNextProcess";

  m_currentCommand = m_commandStack.last();
  m_nextDirectory = m_currentDirectory = m_currentCommand.directory;

  if (m_commandStack.count() > 1)
    m_commandStack.removeLast(); //always make sure we have one app to launch
  delete m_currentProcess;

  m_currentProcess = new PiProcess(this);

  connect(m_currentProcess, SIGNAL(terminated()), this, SLOT(launchNextProcess()), Qt::QueuedConnection);


  //  qDebug() << "termination:" << m_terminationTimer.isActive() << m_terminateNextProcessDelay;
  if (m_terminationTimer.isActive())
    m_terminationTimer.stop();
  if (m_terminateNextProcessDelay > 0)
    m_terminationTimer.start(m_terminateNextProcessDelay, this);
  m_terminateNextProcessDelay = 0;


  static QByteArray defaultPath = qgetenv("PATH");
  
  if (!m_currentCommand.directory.isEmpty()) {
    qDebug() << "working dir" << m_currentCommand.directory;
    m_currentProcess->setWorkingDirectory(m_currentCommand.directory);
    qputenv("PATH", m_currentCommand.directory + ":" + defaultPath);
  }

  qDebug() << "...launching" << m_currentCommand.commandLine;
  m_currentProcess->start(m_currentCommand.commandLine);
  qDebug() << "launched with QProcess::state()" << m_currentProcess->state();
  qputenv("PATH", defaultPath);
}
