#include <cstdint>
#include <string>
#include <vector>

#ifndef PROCESS_HXX
#define PROCESS_HXX

namespace yeetdbg {
  enum STATUS {
    RUNNING,
    PAUSED,
    DEAD,
  };

  class Process {
    public:
      Process() {};
      Process(std::string file, std::vector<std::string> params): m_file(file), m_params(params) {}
      void start();
      void resume();
      int wait(int options);

      uint64_t read_quad(uint64_t addr);
      void write_quad(uint64_t addr, uint64_t data);

      uint64_t m_base = 0;
      std::string maps;
    private:
      void read_mappings();

      int m_pid;
      std::string m_file;
      std::vector<std::string> m_params;
      STATUS m_status;
  };
}

#endif
