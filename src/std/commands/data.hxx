
#ifndef REGISTERS_HXX
#define REGISTERS_HXX

#include <string>
#include <vector>
#include <commands/command.hxx>

namespace cmd {
  class Memory: public Command {
    public:
      Memory(std::Process &p): Command(p) {};
      int handle_command(std::vector<std::string>) override;

      void handle_signal(siginfo_t signal) override {}
      void pre_exec() override {}

      std::string command() const override {
        return "memory";
      };

      std::string description() const override {
        return "Manage memory.\n"
          "\tread: `memory read 0x7f12 4` (Read 4 counts of 8 bytes at 0x7f12. The count can be omitted and defaults to 1.)\n"
          "\twrite: `registers write 0x7f12 0x20` (Write 0x20 to 0x7f12)";
      };

      bool should_wait() const override { return false; }

    private:
  };

  class Registers: public Command {
    public:
      Registers(std::Process &p): Command(p) {};
      int handle_command(std::vector<std::string>) override;

      void handle_signal(siginfo_t signal) override {}
      void pre_exec() override {}

      std::string command() const override {
        return "registers";
      };

      std::string description() const override {
        return "Manage registers.\n"
          "\tread: `registers read rax` (Read the value of rax)\n"
          "\twrite: `registers write rcx 0x20` (Write 0x20 to rcx)";

      };

      bool should_wait() const override { return false; }

    private:
  };
}

#endif
