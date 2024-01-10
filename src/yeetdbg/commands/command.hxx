#ifndef COMMAND_HXX
#define COMMAND_HXX

#include <string>
#include <vector>
#include <process.hxx>

namespace cmd {
  class Command {
    public:
      Command(yeetdbg::Process& p): m_p(p) {};
      virtual int handle_command(std::vector<std::string>) = 0;

      virtual std::string command() const = 0;
      virtual std::string description() const = 0;
      virtual bool should_wait() const = 0;


      virtual ~Command() {}

      protected:
      yeetdbg::Process& m_p;
  };
}

#endif
