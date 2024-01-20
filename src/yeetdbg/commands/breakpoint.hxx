#include "commands/command.hxx"

#include <breakpoint.hxx>
#include <map>
#include <unordered_map>
#include <vector>

#ifndef BREAKPOINT_COMMAND_HXX
#define BREAKPOINT_COMMAND_HXX

namespace cmd {
  class BreakpointManage: public Command {
    public:
      BreakpointManage(yeetdbg::Process &p): Command(p) {};
      int handle_command(std::vector<std::string>) override;

      std::string command() const override {
        return "breakpoint";
      };

      std::string description() const override {
        return "Manage breakpoints.\n"
          "\tadd: `breakpoint add $1149` (Add a breakpoint at 0x1149 offset from the beginning of the loaded executable. If address is not prefixed by '$', it is taken literally. BREAKPOINTS ARE DISABLED WHEN FIRST CREATED)\n"
          "\tlist: `breakpoint list` (List all breakpoints and their status)\n"
          "\tenable: `breapoint enable 0` (Enable the breakpoint with id 0, you can get the id by listing breakpoints)\n"
          "\tdisable: `breakpoint disable 0` (Disable the breakpoint with id 0)\n";
      };

      void handle_signal(siginfo_t signal) override;
      void pre_exec() override;

      bool should_wait() const override { return false; }

    private:
      std::vector<yeetdbg::Breakpoint> breakpoints;
      std::map<uint64_t, yeetdbg::Breakpoint*> bp_addr;

      int enable_bp(int i);
      int disable_bp(int i);
      int del_bp(int i);
      int add_bp(std::string);
  };
}
#endif
