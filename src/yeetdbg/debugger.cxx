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
  int options = 0;
  char* line;

  int wait_status = process.wait(options);

  while((line = linenoise("ydb> ")) != nullptr){
    if(handle_command(line)){
      wait_status = process.wait(options);

      // TODO Figure out a way to enable all breakpoints before startup

      if(wait_status) // Call signal handlers for all commands
        for(auto it = m_commands.begin(); it != m_commands.end(); it++)
          it->get()->handle_signal(wait_status);
    }

    linenoiseHistoryAdd(line);
    linenoiseFree(line);
  }
}

bool Debugger::handle_command(const std::string& command_full) {
  auto cmd_args = split(command_full, ' ');

  if(cmd_args.size() == 0)
    return -1;

  std::string cmd = cmd_args[0];

  for(auto& c : m_commands){
    if(c->command().size() > 0 && is_prefix(cmd, c->command())) {
      std::cout << c->command() << std::endl;

      if(c->should_wait())
        for(auto it = m_commands.begin(); it != m_commands.end(); it++)
          it->get()->pre_exec();

      return c->handle_command(std::vector<std::string>(cmd_args.begin() + 1,
                                                        cmd_args.end())) == 0 &&
             c->should_wait();
    }
  }
  unknown_command(cmd);
  return -1;
}

void Debugger::unknown_command(std::string cmd){
  std::cout << "Command " << cmd << " not found." << std::endl;
}

void Debugger::start_process(){
  process = Process(m_name, std::vector<std::string>());
  process.start();
}
