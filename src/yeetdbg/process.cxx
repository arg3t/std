#include <process.hxx>
#include <string>
#include <utils.hxx>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fstream>
#include <format>
#include <sys/personality.h>

using namespace yeetdbg;

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
    m_status = PAUSED;
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
