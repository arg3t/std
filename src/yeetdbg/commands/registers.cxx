#include <commands/registers.hxx>
#include <iostream>
#include <utils.hxx>

using namespace cmd;

int Registers::handle_command(std::vector<std::string> params){
  if (params.size() == 1 && is_prefix(params[0], "read")){
    std::string printed_regs[] = {
        "rax", "rcx", "rdx", "rsi", "rdi", "r8",  "r9",  "r10",
        "r11", "rbx", "rbp", "r12", "r13", "r14", "r15",
    };

    for(auto &it: printed_regs)
      std::cout << it << ": " << int_to_hex(m_proc.get_reg_value(it)) << std::endl;

  }else if (params.size() == 2 && is_prefix(params[0], "read")){
    try{
      std::cout << params[1] << ": " << int_to_hex(m_proc.get_reg_value(params[1])) << std::endl;
    } catch (int e) {
      if(e == REGISTER_NOT_FOUND_EX)
        std::cout << "Register " << params[1] << " not found." << std::endl;
    }
  }else if(params.size() == 3 && is_prefix(params[0], "write")){
    try{
      std::cout << params[1] << "[OLD]: " << int_to_hex(m_proc.get_reg_value(params[1])) << std::endl;
    } catch (int e) {
      if(e == REGISTER_NOT_FOUND_EX)
        std::cout << "Register " << params[1] << " not found." << std::endl;
    }

    m_proc.set_reg_value(params[1], std::stoll(params[2], 0, 16));
  }else{
    return -1;
  }
  return -1;
}
