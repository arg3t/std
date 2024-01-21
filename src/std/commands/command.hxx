#ifndef COMMAND_HXX
#define COMMAND_HXX

#include <string>
#include <vector>
#include <process.hxx>

namespace cmd {
  class Command {
    public:
      Command(std::Process &p): m_proc(p) {};
      virtual int handle_command(std::vector<std::string>) = 0;

      virtual std::string command() const = 0;
      virtual std::string description() const = 0;
      virtual bool should_wait() const = 0;

      virtual void handle_signal(siginfo_t signal) = 0;
      virtual void pre_exec() = 0;

      virtual ~Command() {}

      protected:
      std::Process &m_proc;
  };
}

#endif
