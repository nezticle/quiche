#include "process.h"
#include "processmanager.h"

#include <qtimer.h>
#include <qdebug.h>

enum {
  WaitForAckDelay = 500,
  WaitAfterAckDelay = 5000,
  WaitBeforeKillDelay = 2000
};



PiProcess::PiProcess(PiProcessManager *parent)
  : QProcess(parent), m_terminationState(None), m_processManager(parent)
{
  setReadChannel(StandardOutput);
  connect(this, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(handleTermination(int, QProcess::ExitStatus)));
  connect(this, SIGNAL(error(QProcess::ProcessError)), this, SLOT(handleError(QProcess::ProcessError)));
  connect(this, SIGNAL(readyRead()), this, SLOT(handleReadyRead()));
}

PiProcess::~PiProcess()
{
  qDebug() << "~PiProcess()";
}

void PiProcess::terminateProcess()
{
  write("TERMINATING\n");

  if (m_terminationState == None) {
    m_terminationState = WaitForAck;
    triggerNextState(WaitForAckDelay);
  }
}

void PiProcess::handleCommand(const QByteArray &line)
{
  if (line == "ACK\n") {
    if (m_terminationState == WaitForAck) {
      m_terminationState = AckReceived;
      triggerNextState(WaitAfterAckDelay);
    }
  } else if (line == "QUEUEME\n") {
    qDebug() << "requeueing";
    m_processManager->requeueCurrent();
  } else if (line.startsWith("QUEUE ")) {
    qDebug() << "queueing";
    m_processManager->queue(line.mid(6));
  } else if (line.startsWith("KILL ")) {
    int delay = line.mid(5).trimmed().toInt();
    qDebug() << "termination request" << delay;
    if (delay > 0)
      m_processManager->terminateNextProcessAfter(delay);
  }

}


void PiProcess::handleReadyRead()
{
  qDebug() << "PiProcess::handleReadyRead";

  QByteArray line = readLine();
  qDebug() << "read:" << line;
  handleCommand(line);

}

void PiProcess::triggerNextState(int delay)
{
    QTimer::singleShot(delay, this, SLOT(terminationStateMachineSlot()));
}

void PiProcess::terminationStateMachineSlot()
{
  if (m_terminationState == WaitForAck    //timed out waiting for ack
  || m_terminationState == AckReceived) { // or timed out waiting for app to exit
    terminate();
    m_terminationState = Terminated;
    triggerNextState(WaitBeforeKillDelay);
  } else if (m_terminationState == Terminated) {
    //timed out waiting for app to terminate
    kill();
    m_terminationState = Killed;
  }
}


void PiProcess::handleError(ProcessError err)
{
  qDebug() << "ERROR" << err;
  if (err == QProcess::FailedToStart)
    emit terminated();
}

void PiProcess::handleTermination(int exitCode, ExitStatus exitStatus)
{
  qDebug() << "TERMINATED" << exitCode << exitStatus;
  m_terminationState = Dead;
  while (canReadLine()) {
    QByteArray line = readLine();
    qDebug() << "Read:" << line;
    handleCommand(line);
  }
  emit terminated();
}
