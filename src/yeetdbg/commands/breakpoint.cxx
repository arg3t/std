#include "commands/breakpoint.hxx"

#include <iostream>
#include <stdexcept>
#include <utils.hxx>
#include <vector>

using namespace cmd;

int BreakpointManage::handle_command(std::vector<std::string> cmd) {
  if (cmd.size() <= 1) {
    std::cout << "Command breakpoint needs at least two arguments!"
              << std::endl;
    return -1;
  }

  auto subcmd = cmd.at(0);
  auto param = cmd.at(1);

  if (is_prefix(subcmd, "add")) {
    return add_bp(param);
  } else if (is_prefix(subcmd, "enable")) {
    try {
      return enable_bp(stoi(param));
    } catch (std::invalid_argument ex) {
      std::cout << "'" << param << "' is not a valid integer" << std::endl;
      return -1;
    }
  } else if (is_prefix(subcmd, "disable")) {
    try {
      return disable_bp(stoi(param));
    } catch (std::invalid_argument ex) {
      std::cout << "'" << param << "' is not a valid integer" << std::endl;
      return -1;
    }
  } else if (is_prefix(subcmd, "list")) {
  } else if (is_prefix(subcmd, "remove")) {
    try {
      return del_bp(stoi(param));
    } catch (std::invalid_argument ex) {
      std::cout << "'" << param << "' is not a valid integer" << std::endl;
      return -1;
    }
  } else {
    std::cout << "'" << subcmd << "' is not a valid subcommand" << std::endl;
    return -1;
  }

  return 0;
}

int BreakpointManage::add_bp(std::string addr_str) {
  try {
    return enable_bp(stoi(addr_str));
  } catch (std::invalid_argument ex) {
    std::cout << "'" << param << "' is not a valid integer" << std::endl;
    return -1;
  }
}

int BreakpointManage::enable_bp(int i) {}

int BreakpointManage::disable_bp(int i) {}

int BreakpointManage::del_bp(int i) {}
