#include "processmanager.h"
#include "process.h"

#include <qtimer.h>
#include <qdebug.h>


PiProcessManager::PiProcessManager(int argc, char**argv)
  :QCoreApplication(argc, argv)
  , m_currentProcess(0), m_terminateNextProcessDelay(-1)
{
  QTimer::singleShot(0, this, SLOT(launchNextProcess()));
}

PiProcessManager::~PiProcessManager()
{
  delete m_currentProcess;
}

void PiProcessManager::queue(const QByteArray &commandLine)
{
  qDebug() << "queueing" << commandLine;
  m_commandLineStack.append(commandLine);
}

void PiProcessManager::requeueCurrent()
{
  qDebug() << "requeueing" << m_currentCommandLine;
  m_commandLineStack.append(m_currentCommandLine);
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
    qDebug() << "PiProcessManager::timerEvent() terminating" << m_currentCommandLine;
    terminateCurrentProcess();
  }
}



void PiProcessManager::launchNextProcess()
{
  qDebug() << "PiProcessManager::launchNextProcess";

  m_currentCommandLine = m_commandLineStack.last();

  if (m_commandLineStack.count() > 1)
    m_commandLineStack.removeLast(); //always make sure we have one app to launch
  delete m_currentProcess;

  m_currentProcess = new PiProcess(this);

  connect(m_currentProcess, SIGNAL(terminated()), this, SLOT(launchNextProcess()), Qt::QueuedConnection);


  qDebug() << "termination:" << m_terminationTimer.isActive() << m_terminateNextProcessDelay;
  if (m_terminationTimer.isActive())
    m_terminationTimer.stop();
  if (m_terminateNextProcessDelay > 0)
    m_terminationTimer.start(m_terminateNextProcessDelay, this);
  m_terminateNextProcessDelay = 0;
  
  qDebug() << "...launching" << m_currentCommandLine;
  m_currentProcess->start(m_currentCommandLine);
  qDebug() << "launched with QProcess::state()" << m_currentProcess->state();

}
