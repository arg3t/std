#include <iostream>
#include <unistd.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <vector>

#include "debugger.hxx"
#include "utils.hxx"
#include "linenoise.h"

using namespace yeetdbg;

void Debugger::run(){
  int wait_status;
  int options = 0;

  char* line;

  waitpid(m_pid, &wait_status, options);

  while((line = linenoise("ydb> ")) != nullptr){
    handle_command(line);
    linenoiseHistoryAdd(line);
    linenoiseFree(line);
    waitpid(m_pid, &wait_status, options);
  }
}

void Debugger::handle_command(const std::string& command_full) {
  auto cmd_args = split(command_full, ' ');

  if(cmd_args.size() == 0)
    return;

  std::string cmd = cmd_args[0];

  for(auto& c : m_commands){
    if(is_prefix(cmd, c->command())) {
      std::cout << c->command() << std::endl;
      c->handle_command(std::vector<std::string>(cmd_args.begin() + 1, cmd_args.end()));
      return;
    }
  }
  unknown_command(cmd);
}

void Debugger::unknown_command(std::string cmd){
  std::cout << "Command " << cmd << " not found." << std::endl;
}

Debugger Debugger::start_process(std::string name){
}
