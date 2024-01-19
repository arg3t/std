#ifndef CONTINUE_HXX
#define CONTINUE_HXX

#include <string>
#include <vector>
#include <commands/command.hxx>

namespace cmd {
  class Continue: public Command {
    public:
      Continue(yeetdbg::Process &p): Command(p) {};
      int handle_command(std::vector<std::string>) override;

      void handle_signal(uint64_t signal) override {}
      void pre_exec() override {}

      std::string command() const override {
        return "continue";
      };

      std::string description() const override {
        return "Continue execution.";
      };

      bool should_wait() const override { return true; }

    private:
  };
}

#endif
