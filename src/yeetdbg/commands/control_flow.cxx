#include <iostream>
#include <string>
#include <vector>
#include <sys/ptrace.h>
#include <sys/wait.h>

#include "commands/control_flow.hxx"

using namespace cmd;

int Continue::handle_command(std::vector<std::string> params) {
  m_proc.resume();
  return 0;
}
