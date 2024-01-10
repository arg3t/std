#include <cstdint>

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
      Breakpoint(uint64_t addr, uint8_t pid);
      uint64_t get_addr() { return m_addr; }
      Status is_enabled() { return status; }

      void enable();
      void disable();

    private:
      uint64_t m_addr;
      uint8_t m_pid;

      uint64_t old_data;
      Status status;
  };
}
#endif
