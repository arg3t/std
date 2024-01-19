#include <breakpoint.hxx>
#include <sys/ptrace.h>

using namespace yeetdbg;

void Breakpoint::enable(){
  if(old_data_populated)
    return;
  /* Fetch old instruction data in old point */
  old_data = m_proc->read_quad(get_addr());
  uint64_t injected_interrupt = (old_data & ~0xff) | INT3;
  old_data_populated = true;

  /* Replace instruction with int3 */

  m_proc->write_quad(get_addr(), injected_interrupt);
}

uint64_t Breakpoint::get_addr(){
  if(m_relative)
    return m_proc->offset_by_base(m_addr);
  else
    return m_addr;
}

void Breakpoint::disable(){
  m_proc->write_quad(get_addr(), old_data);
  old_data_populated = false;
}
