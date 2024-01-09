#include <cstdint>

namespace yeetdbg {
  class Breakpoint {
    public:
      Breakpoint(uint64_t addr, uint8_t pid): m_addr(addr), m_pid(pid) {}
      uint64_t get_addr() { return m_addr; }
      bool is_enabled() { return status; }

      void enable();
      void disable();

    private:
      uint64_t m_addr;
      uint8_t m_pid;

      uint8_t old_data;
      bool status;
  };
}
