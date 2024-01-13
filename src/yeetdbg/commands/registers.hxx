
#ifndef REGISTERS_HXX
#define REGISTERS_HXX

#include <string>
#include <vector>
#include <commands/command.hxx>

namespace cmd {
  class Registers: public Command {
    public:
      Registers(yeetdbg::Process &p): Command(p) {};
      int handle_command(std::vector<std::string>) override;

      void handle_signal(uint64_t signal) override {}
      void pre_exec() override {}

      std::string command() const override {
        return "registers";
      };

      std::string description() const override {
        return "Manage registers";
      };

      bool should_wait() const override { return false; }

    private:
  };
}

#endif