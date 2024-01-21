#ifndef CONTINUE_HXX
#define CONTINUE_HXX

#include <string>
#include <vector>
#include <commands/command.hxx>

namespace cmd {

  class Continue: public Command {
    public:
      Continue(std::Process &p): Command(p) {};
      int handle_command(std::vector<std::string>) override {
        m_proc.resume();
        return 0;
      }

      void handle_signal(siginfo_t signal) override {}
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

  class Step: public Command {
    public:
      Step(std::Process &p): Command(p) {};
      int handle_command(std::vector<std::string> params) override {
        int count = 0;
        if(params.size() > 0){
          count = std::stoi(params[1]);
        }else{
          count = 1;
        }

        for(int i = 0; i < count; i++)
          m_proc.step_instruction();

        return 0;
      }

      void handle_signal(siginfo_t signal) override {}
      void pre_exec() override {}

      std::string command() const override {
        return "step";
      };

      std::string description() const override {
        return "Step a number of instructions. Run `step 3` to step 3 instructions.";
      };

      bool should_wait() const override { return true; }

    private:
  };
}

#endif
