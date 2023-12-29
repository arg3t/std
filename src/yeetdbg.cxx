#include <iostream>
#include <unistd.h>
#include <sys/ptrace.h>
#include <sys/wait.h>

#include "yeetdbg.hxx"
#include "utils.hxx"
#include "linenoise.h"

using namespace yeetdbg;

void Debugger::run(){
  int wait_status;
  int options = 0;

  waitpid(m_pid, &wait_status, options);
  char* line;

  while((line = linenoise("ydb> ")) != nullptr){
    handle_command(line);
    linenoiseHistoryAdd(line);
    linenoiseFree(line);
  }
}

void Debugger::handle_command(const std::string& command_full) {
  auto cmd_args = utils::split(command_full, ' ');
  std::string cmd = cmd_args[0];

  if(utils::is_prefix(cmd, "continue")) {
    continue_execution();
  } else {
    unknown_command(cmd);
  }
}

void Debugger::continue_execution(){
  ptrace(PTRACE_CONT, m_pid, nullptr, nullptr);
}

void Debugger::unknown_command(std::string cmd){
  std::cout << "Command " << cmd << " not found." << std::endl;
}


int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cerr << "You must provide an executable name!" << std::endl;
    return 1;
  }

  char* name = argv[1];

  int pid = fork();

  if (pid == 0) {
    // Enable debugging on child and start executable
    ptrace(PTRACE_TRACEME, 0, nullptr, nullptr);
    execl(name, name, nullptr);
  } else {
    std::cout << "Starting debugging " << name << " with PID: " << pid << std::endl;

    Debugger debugger{name, pid};
    debugger.run();
  }
}
