#include <commands/breakpoint.hxx>

#include <iostream>
#include <stdexcept>
#include <utils.hxx>
#include <vector>
#include <tableau.hxx>

using namespace cmd;

int BreakpointManage::handle_command(std::vector<std::string> cmd) {
  if (!(cmd.size() == 1 && is_prefix(cmd.at(0), "list")) && cmd.size() != 2) {
    std::cout << "Command breakpoint needs two arguments!"
              << std::endl;
    return -1;
  }

  auto subcmd = cmd.at(0);
  auto param = cmd.size() == 2 ? cmd.at(1) : "";

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
    auto t = new yeetdbg::Tableau<int, yeetdbg::Status, std::string, long>({"Index", "Status", "Address", "Old Data"});

    int c = 0;
    for(auto &it : breakpoints){
      t->add_row(c++, it.is_enabled(), int_to_hex(it.get_addr()), 0);
    }

    std::cout << t->to_string();
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

void print_breakpoints(std::vector<yeetdbg::Breakpoint> breakpoints){
  for(auto & b : breakpoints){
  }
}

int BreakpointManage::add_bp(std::string addr_str) {
  try {
    auto bp = yeetdbg::Breakpoint(stoi(addr_str, 0, 16), m_pid);
    breakpoints.push_back(bp);
  } catch (std::invalid_argument ex) {
    std::cout << "Unable to parse address " << addr_str << std::endl;
    return -1;
  }

  return 0;
}

int BreakpointManage::enable_bp(int i) {
  if(breakpoints.size() <= i){
    std::cout << "Breakpoint with idx " << i << " does not exist!" << std::endl;
    return -1;
  }

  breakpoints.at(i).enable();
  return 0;
}

int BreakpointManage::disable_bp(int i) {
  if(breakpoints.size() <= i){
    std::cout << "Breakpoint with idx " << i << " does not exist!" << std::endl;
    return -1;
  }

  breakpoints.at(i).disable();
  return 0;
}

int BreakpointManage::del_bp(int i) {
  if(breakpoints.size() <= i){
    std::cout << "Breakpoint with idx " << i << " does not exist!" << std::endl;
    return -1;
  }

  breakpoints.at(i).disable();
  breakpoints.erase(breakpoints.begin() + i);

  return 0;
}
