#include <iostream>
#include <unistd.h>
#include <sys/ptrace.h>
#include <sys/wait.h>

#include "debugger.hxx"
#include "utils.hxx"

using namespace std;

int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cerr << "You must provide an executable name!" << std::endl;
    return 1;
  }

  char* name = argv[1];

  Debugger debugger = Debugger(name);
  debugger.start_process();
  debugger.run();
}
