#include <cstring>
#include <iostream>
#include <unistd.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <vector>

#include "debugger.hxx"
#include "utils.hxx"
#include "linenoise.h"

using namespace yeetdbg;

void main_signal_handler(int sig){
  if(WIFEXITED(sig)){
    std::cout << ">> Process exited with status code " << WEXITSTATUS(sig) << std::endl;
  }else if(WIFSIGNALED(sig)){
    std::cout << ">> Process terminated with signal SIG" << sigabbrev_np(WTERMSIG(sig)) << "[" << WTERMSIG(sig) << "]" << std::endl;
  }else if(WIFSTOPPED(sig)){
    std::cout << ">> Process stopped with signal SIG" << sigabbrev_np(WSTOPSIG(sig)) << "[" << WSTOPSIG(sig) << "]" << std::endl;
  }else if(WIFCONTINUED(sig)){
    std::cout << ">> Process stopped properly after continue" << std::endl;
  }else{
    std::cout << ">> Unknown signal " << int_to_hex(sig) << std::endl;
  }
}

void Debugger::run(){
  int options = 0;
  char* line;

  int wait_status = process.wait(options);

  main_signal_handler(wait_status);

  while((line = linenoise("ydb> ")) != nullptr){
    if(handle_command(line)){
      wait_status = process.wait(options);

      main_signal_handler(wait_status);

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
      std::cout << ">> " << c->command() << std::endl;

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
