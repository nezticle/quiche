#include "processmanager.h"

int main(int argc, char **argv)
{

  PiProcessManager pm(argc,argv);

  const char *launcher = "bash"; //well..

  if (argc > 1)
    launcher = argv[1];

  pm.queue(launcher);


  return pm.exec();
}
