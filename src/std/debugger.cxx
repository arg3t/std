#include <csignal>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <vector>

#include "debugger.hxx"
#include "utils.hxx"
#include "linenoise.h"

using namespace std;

void main_signal_handler(siginfo_t sig){
  std::cout << ">> " << strsignal(sig.si_signo) << "[" << sig.si_signo << "]" << std::endl;
}

void Debugger::run(){
  int options = 0;
  char* line;

  int wait_status = process.wait(options);

  main_signal_handler(process.get_siginfo());
  std::cout << "Type in \"help\" to read help text" << std::endl;

  while((line = linenoise("std> ")) != nullptr){
    if(handle_command(line)){
      wait_status = process.wait(options);
      try{
        auto s = process.get_siginfo();

        main_signal_handler(s);

        if(wait_status) // Call signal handlers for all commands
          for(auto it = m_commands.begin(); it != m_commands.end(); it++)
            it->get()->handle_signal(s);

        process.handle_signal(s);
      }catch(int){
        continue;
      }
    }

    linenoiseHistoryAdd(line);
    linenoiseFree(line);
    std::cout.flush();
  }
}

bool Debugger::handle_command(const std::string& command_full) {
  auto cmd_args = split(command_full, ' ');

  if(cmd_args.size() == 0)
    return -1;

  std::string cmd = cmd_args[0];

  if(is_prefix(cmd, "help")){
    for(auto& c : m_commands){
      std::cout << "[" << c->command() << "]:" << std::endl;
      std::cout << c->description() << std::endl << std::endl;
    }
    return false;
  }

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
