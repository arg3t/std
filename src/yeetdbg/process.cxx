#include <algorithm>
#include <cstdint>
#include <iostream>
#include <process.hxx>
#include <string>
#include <sys/user.h>
#include <type_traits>
#include <utils.hxx>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fstream>
#include <format>
#include <sys/personality.h>

using std::enable_if_t;

using namespace yeetdbg;

uint8_t find_reg_idx(reg v) {
  auto it = std::find_if(g_registers.begin(), g_registers.end(),
                         [v](reg_descriptor rd) { return rd.r == v; });

  if(it == g_registers.end())
    throw REGISTER_NOT_FOUND_EX;

  return it - g_registers.begin();
}

uint8_t find_reg_idx(int v) {
  auto it = std::find_if(g_registers.begin(), g_registers.end(),
                         [v](reg_descriptor rd) { return rd.dwarfno == v; });

  if(it == g_registers.end())
    throw REGISTER_NOT_FOUND_EX;

  return it - g_registers.begin();
}

uint8_t find_reg_idx(std::string v) {
  auto it = std::find_if(g_registers.begin(), g_registers.end(),
                         [v](reg_descriptor rd) { return rd.name == v; });

  if(it == g_registers.end())
    throw REGISTER_NOT_FOUND_EX;

  return it - g_registers.begin();
}


void Process::start(){
  int pid = fork();
  int wait_status;

  if (pid == 0) {
    // Enable debugging on child and start executable
    ptrace(PTRACE_TRACEME, 0, nullptr, nullptr);
    personality(ADDR_NO_RANDOMIZE); // Disable ASLR

    // TODO Change this so that it supports parameters
    execl(m_file.c_str(), m_file.c_str(), nullptr);
    exit(0);
  } else {
    m_pid = pid;
    waitpid(m_pid, &wait_status, 0);
    std::cout << "Started process with PID " << m_pid << std::endl;
    m_status = PAUSED;
    read_mappings();
  }
}

int Process::wait(int options = 0){
  int wait_status;

  if(m_status == PAUSED)
    return 0;

  waitpid(m_pid, &wait_status, options);
  m_status = PAUSED;

  return wait_status;
}

void Process::resume(){
  m_status = RUNNING;
  ptrace(PTRACE_CONT, m_pid, nullptr, nullptr);
}

void Process::read_mappings(){
  std::ifstream fmaps(std::format("/proc/{}/maps", m_pid));
  std::string line;

  while(fmaps.good()){
    fmaps >> line;
    if(m_base == 0){
      m_base = std::stol(split(line, '-').at(0), 0, 16);
    }
    maps += line + std::string("\n");
  }
}

uint64_t Process::read_quad(uint64_t addr){
  return ptrace(PTRACE_PEEKDATA, m_pid, addr);
}

void Process::write_quad(uint64_t addr, uint64_t data){
  ptrace(PTRACE_POKEDATA, m_pid, addr, data);
}


uint64_t Process::get_reg_by_idx(int r){
  user_regs_struct regs;

  ptrace(PTRACE_GETREGS, m_pid, nullptr, &regs);
  return *(reinterpret_cast<uint64_t*>(&regs) + r);
}

void Process::set_reg_by_idx(int r, uint64_t data){
  user_regs_struct regs;

  ptrace(PTRACE_GETREGS, m_pid, nullptr, &regs);
  *(reinterpret_cast<uint64_t*>(&regs) + r) = data;

  ptrace(PTRACE_SETREGS, m_pid, nullptr, &regs);
}

uint64_t Process::get_reg_value(reg r) { return get_reg_by_idx(find_reg_idx(r)); }
void Process::set_reg_value(reg r, uint64_t data) { set_reg_by_idx(find_reg_idx(r), data); }

uint64_t Process::get_reg_value(int r) { return get_reg_by_idx(find_reg_idx(r)); }
void Process::set_reg_value(int r, uint64_t data) { set_reg_by_idx(find_reg_idx(r), data); }

uint64_t Process::get_reg_value(std::string r) { return get_reg_by_idx(find_reg_idx(r)); }
void Process::set_reg_value(std::string r, uint64_t data) { set_reg_by_idx(find_reg_idx(r), data); }


void Process::step_instruction(uint8_t count){
  if(m_status == DEAD)
    return;

  if(count > 0){
    m_status = RUNNING;
    ptrace(PTRACE_SINGLESTEP, m_pid, nullptr, nullptr);
    wait();
    step_instruction(count - 1);
  }
}
