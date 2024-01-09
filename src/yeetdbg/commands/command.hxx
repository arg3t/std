#ifndef COMMAND_HXX
#define COMMAND_HXX

#include <string>
#include <vector>

namespace cmd {
  class Command {
    public:
      Command(int pid): m_pid(pid) {};
      virtual int handle_command(std::vector<std::string>) = 0;

      virtual std::string command() const = 0;
      virtual std::string description() const = 0;
      virtual bool should_wait() const = 0;


      virtual ~Command() {}

      protected:
      int m_pid;
  };
}

#endif
