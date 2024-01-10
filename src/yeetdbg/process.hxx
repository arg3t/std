#include <cstdint>
#include <string>
#include <vector>

#ifndef PROCESS_HXX
#define PROCESS_HXX

namespace yeetdbg {
  enum STATUS {
    RUNNING,
    WAITING,
    DEAD,
  };

  class Process {
    public:
      Process(std::string file, std::vector<std::string> params): m_file(file), m_params(params) {}
      void start();
      void resume();

      uint64_t read_quad(uint64_t addr);
      void write_quad(uint64_t addr, uint64_t data);

      uint64_t m_base;
    private:
      int m_pid;
      std::string maps;
      std::string m_file;
      std::vector<std::string> m_params;
  };
}

#endif
