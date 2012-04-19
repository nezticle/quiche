#include <qprocess.h>

class PiProcessManager;

class PiProcess : public QProcess
{
Q_OBJECT
public:
  PiProcess(PiProcessManager *parent);
  ~PiProcess();
  void terminateProcess();

  enum TerminationState {None, WaitForAck, AckReceived, Terminated, Killed, Dead};
  
signals:
  void terminated();

protected slots:

  void handleReadyRead();
  void handleError(QProcess::ProcessError);
  void handleTermination(int exitCode, QProcess::ExitStatus exitStatus);

private slots:

  void terminationStateMachineSlot();

private:
  void triggerNextState(int delay);
  void handleCommand(const QByteArray &line);

  TerminationState m_terminationState;
  PiProcessManager *m_processManager;
};
