#include <qcoreapplication.h>
#include <qbytearray.h>
#include <qlist.h>
#include <qbasictimer.h>

class PiProcess;

class PiProcessManager : public QCoreApplication
{
Q_OBJECT
public:
  PiProcessManager(int argc, char**argv);
  ~PiProcessManager();

  void queue(const QByteArray &commandLine);
  void requeueCurrent();

public slots:
  void terminateCurrentProcess();
  void terminateNextProcessAfter(int msec);

protected slots:
  void launchNextProcess();

protected:
  void timerEvent(QTimerEvent * event);


private:
  QList<QByteArray> m_commandLineStack;
  QByteArray m_currentCommandLine;

  PiProcess *m_currentProcess;

  QBasicTimer m_terminationTimer;
  int m_terminateNextProcessDelay;
};
