#include <commands/breakpoint.hxx>

#include <iostream>
#include <stdexcept>
#include <string>
#include <tableau.hxx>
#include <utils.hxx>
#include <vector>

using namespace cmd;

int BreakpointManage::handle_command(std::vector<std::string> cmd) {
  if (!(cmd.size() == 1 && is_prefix(cmd.at(0), "list")) && cmd.size() != 2) {
    std::cout << ">> Command breakpoint needs two arguments!" << std::endl;
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
      std::cout << ">> '" << param << "' is not a valid integer" << std::endl;
      return -1;
    }
  } else if (is_prefix(subcmd, "disable")) {
    try {
      return disable_bp(stoi(param));
    } catch (std::invalid_argument ex) {
      std::cout << ">> '" << param << "' is not a valid integer" << std::endl;
      return -1;
    }
  } else if (is_prefix(subcmd, "list")) {
    auto t = new yeetdbg::Tableau<int, yeetdbg::Status, std::string, std::string, bool>(
        {"Index", "Status", "Address", "Old Data", "Is Relative"});

    int c = 0;

    for (auto &b : breakpoints) {
      t->add_row(c++, b.get_status(), int_to_hex(b.get_addr()), int_to_hex(b.get_old_data()), b.is_relative());
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

void print_breakpoints(std::vector<yeetdbg::Breakpoint> breakpoints) {
  for (auto &b : breakpoints) {
  }
}

int BreakpointManage::add_bp(std::string addr_str) {
  uint64_t addr;
  bool relative;

  try {
    if(addr_str.length() > 0 && addr_str.at(0) == '$'){
      addr_str.erase(0, 1);
      addr = std::stoll(addr_str, 0, 16);
      relative = true;
    } else {
      addr = stoll(addr_str, 0, 16);
      relative = false;
    }

    auto bp = yeetdbg::Breakpoint(addr, &m_proc, relative);
    breakpoints.push_back(bp);
    bp_addr[bp.get_addr()] = &breakpoints.back();

  } catch (std::invalid_argument ex) {
    std::cout << "Unable to parse address " << addr_str << std::endl;
    return -1;
  }

  return 0;
}

int BreakpointManage::enable_bp(int i) {
  if (breakpoints.size() <= i) {
    std::cout << "Breakpoint with idx " << i << " does not exist!" << std::endl;
    return -1;
  }

  breakpoints.at(i).set_status(yeetdbg::ENABLED);
  return 0;
}

int BreakpointManage::disable_bp(int i) {
  if (breakpoints.size() <= i) {
    std::cout << "Breakpoint with idx " << i << " does not exist!" << std::endl;
    return -1;
  }

  breakpoints.at(i).set_status(yeetdbg::DISABLED);
  return 0;
}

int BreakpointManage::del_bp(int i) {
  if (breakpoints.size() <= i) {
    std::cout << ">> Breakpoint with idx " << i << " does not exist!" << std::endl;
    return -1;
  }

  auto bp = breakpoints.at(i);
  bp.disable();

  bp_addr.erase(bp.get_addr());
  breakpoints.erase(breakpoints.begin() + i);

  return 0;
}


void BreakpointManage::handle_signal(uint64_t signal){
  for(auto &bp : breakpoints){
    if(bp.get_status() == yeetdbg::ENABLED){
      bp.disable();
    }
  }
}


void BreakpointManage::pre_exec() {
  auto pc = m_proc.get_reg_value(yeetdbg::reg::rip);
  auto prev_instruction = pc - 1;

  if(bp_addr.count(prev_instruction)){
    m_proc.set_reg_value(yeetdbg::reg::rip, prev_instruction);
    m_proc.step_instruction(1);
  }

  // TODO Sort by address to prevent overlaps
  for(auto &bp : breakpoints){
    if(bp.get_status() == yeetdbg::ENABLED){
      bp.enable();
    }
  }
}

