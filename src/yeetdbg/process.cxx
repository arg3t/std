#include <process.hxx>
#include <sys/ptrace.h>
#include <unistd.h>

using namespace yeetdbg;

void Process::start(){
  int pid = fork();

  if (pid == 0) {
    // Enable debugging on child and start executable
    ptrace(PTRACE_TRACEME, 0, nullptr, nullptr);
    // TODO Change this so that it supports parameters
    execl(m_file.c_str(), m_file.c_str(), nullptr);
    exit(0);
  } else {
    m_pid = pid;
  }
}

void Process::resume(){
  ptrace(PTRACE_CONT, m_pid, nullptr, nullptr);
}

uint64_t Process::read_quad(uint64_t addr){

}

void Process::write_quad(uint64_t addr, uint64_t data){

}
