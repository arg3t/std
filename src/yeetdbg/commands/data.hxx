
#ifndef REGISTERS_HXX
#define REGISTERS_HXX

#include <string>
#include <vector>
#include <commands/command.hxx>

namespace cmd {
  class Memory: public Command {
    public:
      Memory(yeetdbg::Process &p): Command(p) {};
      int handle_command(std::vector<std::string>) override;

      void handle_signal(siginfo_t signal) override {}
      void pre_exec() override {}

      std::string command() const override {
        return "memory";
      };

      std::string description() const override {
        return "Manage memory";
      };

      bool should_wait() const override { return false; }

    private:
  };

  class Registers: public Command {
    public:
      Registers(yeetdbg::Process &p): Command(p) {};
      int handle_command(std::vector<std::string>) override;

      void handle_signal(siginfo_t signal) override {}
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
