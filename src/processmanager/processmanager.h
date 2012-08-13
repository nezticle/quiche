#include <qcoreapplication.h>
#include <qbytearray.h>
#include <qlist.h>
#include <qbasictimer.h>

class PiProcess;


struct PiCommand
{
  PiCommand() {}
  PiCommand(const QByteArray &cmd)
    : commandLine(cmd), directory(QByteArray())
  {
  }
  PiCommand(const QByteArray &cmd, const QByteArray &dir)
    : commandLine(cmd), directory(dir)
  {
  }
  QByteArray commandLine;
  QByteArray directory;
};

class PiProcessManager : public QCoreApplication
{
Q_OBJECT
public:
  PiProcessManager(int argc, char**argv);
  ~PiProcessManager();

  void queue(const QByteArray &commandLine);
  void requeueCurrent();
  void setDirectory(const QByteArray& dir);

public slots:
  void terminateCurrentProcess();
  void terminateNextProcessAfter(int msec);

protected slots:
  void launchNextProcess();

protected:
  void timerEvent(QTimerEvent * event);


private:
  QList<PiCommand> m_commandStack;
  PiCommand m_currentCommand;
  QByteArray m_nextDirectory;
  QByteArray m_currentDirectory;

  PiProcess *m_currentProcess;

  QBasicTimer m_terminationTimer;
  int m_terminateNextProcessDelay;
};
