#ifndef YEETDBG_HXX
#define YEETDBG_HXX

#include "commands/control_flow.hxx"
#include "commands/breakpoint.hxx"
#include "commands/data.hxx"
#include <memory>
#include <string>

namespace yeetdbg {

  class Debugger {
    public:
      Debugger(std::string name): m_name(std::move(name)) {
        m_commands = std::vector<std::shared_ptr<cmd::Command>>{
          std::make_shared<cmd::Continue>(cmd::Continue{process}),
          std::make_shared<cmd::Registers>(cmd::Registers{process}),
          std::make_shared<cmd::Memory>(cmd::Memory{process}),
          std::make_shared<cmd::BreakpointManage>(cmd::BreakpointManage{process})
        };
      }

      void start_process();

      void run();

    private:
      bool handle_command(const std::string& command);
      void unknown_command(std::string cmd);

      std::string m_name;
      Process process;
      std::vector<std::shared_ptr<cmd::Command>> m_commands;
  };

}

#endif
