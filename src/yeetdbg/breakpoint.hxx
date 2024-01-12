#include <cstdint>
#include <memory>
#include <process.hxx>

#ifndef BREAKPOINT_HXX
#define BREAKPOINT_HXX

#define INT3 0x33

namespace yeetdbg {
  enum Status {
    ENABLED,
    DISABLED
  };

  class Breakpoint {
    public:
      Breakpoint(uint64_t addr, Process *proc, bool relative): m_addr(addr), m_proc(proc), m_relative(relative), status(DISABLED) {}
      uint64_t get_addr();
      uint64_t get_old_data() { return old_data; }
      Status is_enabled() { return status; }
      bool is_relative() { return m_relative; }

      void enable();
      void disable();
      void tmp_disable();

    private:
      uint64_t m_addr;
      bool m_relative;
      Process *m_proc;

      uint64_t old_data = 0;
      Status status;
  };
}
#endif
