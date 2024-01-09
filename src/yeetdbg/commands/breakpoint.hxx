#include "commands/command.hxx"

#include <breakpoint.hxx>
#include <vector>

#ifndef BREAKPOINT_COMMAND_HXX
#define BREAKPOINT_COMMAND_HXX

namespace cmd {
  class BreakpointManage: public Command {
    public:
      BreakpointManage(int pid): Command(pid) {};
      int handle_command(std::vector<std::string>) override;

      std::string command() const override {
        return "breakpoint";
      };

      std::string description() const override {
        return "Manage breakpoints";
      };

      bool should_wait() const override { return false; }

    private:
      std::vector<yeetdbg::Breakpoint> breakpoints;

      int enable_bp(int i);
      int disable_bp(int i);
      int del_bp(int i);
      int add_bp(std::string);
  };
}
#endif
