#include <breakpoint.hxx>
#include <sys/ptrace.h>

using namespace yeetdbg;

Breakpoint::Breakpoint(uint64_t addr, uint8_t pid){
  m_addr = addr;
  m_pid = pid;
  status = DISABLED;
}

void Breakpoint::enable(){
  if(status == ENABLED)
    return;

  /* Fetch old instruction data in old point */
  old_data = ptrace(PTRACE_PEEKDATA, m_pid, m_addr);
  uint64_t injected_interrupt = (old_data & ~0xff) | INT3;

  /* Replace instruction with int3 */

  if(ptrace(PTRACE_POKEDATA, m_pid, m_addr, injected_interrupt))
    status = ENABLED;
}

void Breakpoint::disable(){
  if(status == DISABLED)
    return;

  /* Replace instruction with int3 */
  if(ptrace(PTRACE_POKEDATA, m_pid, m_addr, old_data))
    status = DISABLED;
}
