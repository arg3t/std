#ifndef YEETDBG_HXX
#define YEETDBG_HXX

#include <string>

namespace yeetdbg {

  class Debugger {
    public:
      Debugger(std::string name, int pid): m_name(std::move(name)), m_pid(pid) {}

      void run();

    private:
      void handle_command(const std::string& command);
      void continue_execution();
      void unknown_command(std::string cmd);

      std::string m_name;
      int m_pid;
  };

}

#endif
