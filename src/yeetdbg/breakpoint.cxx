#include <breakpoint.hxx>
#include <sys/ptrace.h>

using namespace yeetdbg;

void Breakpoint::enable(){
  if(status == ENABLED)
    return;

  /* Fetch old instruction data in old point */
  old_data = m_proc->read_quad(get_addr());
  uint64_t injected_interrupt = (old_data & ~0xff) | INT3;

  /* Replace instruction with int3 */

  m_proc->write_quad(get_addr(), injected_interrupt);
  status = ENABLED;
}

uint64_t Breakpoint::get_addr(){
  if(m_relative)
    return m_addr + m_proc->m_base;
  else
    return m_addr;
}

void Breakpoint::disable(){
  if(status == DISABLED)
    return;

  /* Replace instruction with int3 */
  m_proc->write_quad(get_addr(), old_data);
  status = DISABLED;
}
