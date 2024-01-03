#ifndef YEETDBG_HXX
#define YEETDBG_HXX

#include "commands/continue.hxx"
#include <memory>
#include <string>

namespace yeetdbg {

  class Debugger {
    public:
      Debugger(std::string name, int pid): m_name(std::move(name)), m_pid(pid) {
        m_commands = std::vector<std::shared_ptr<cmd::Command>>{
          std::make_shared<cmd::Continue>(cmd::Continue{m_pid})
        };
      }
      static Debugger start_process(std::string name);

      void run();

    private:
      void handle_command(const std::string& command);
      void unknown_command(std::string cmd);

      std::string m_name;
      int m_pid;
      std::vector<std::shared_ptr<cmd::Command>> m_commands;
  };

}

#endif
