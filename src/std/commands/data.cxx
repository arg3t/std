#include <commands/data.hxx>
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
      std::cout << ">> " << it << ": " << int_to_hex(m_proc.get_reg_value(it)) << std::endl;

    return 0;
  }else if (params.size() == 2 && is_prefix(params[0], "read")){
    try{
      std::cout << params[1] << ": " << int_to_hex(m_proc.get_reg_value(params[1])) << std::endl;
    } catch (int e) {
      if(e == REGISTER_NOT_FOUND_EX)
        std::cout << ">> Register " << params[1] << " not found." << std::endl;
      return -1;
    }
    return 0;
  }else if(params.size() == 3 && is_prefix(params[0], "write")){
    try{
      std::cout << ">> " << params[1] << "[OLD]: " << int_to_hex(m_proc.get_reg_value(params[1])) << std::endl;
    } catch (int e) {
      if(e == REGISTER_NOT_FOUND_EX)
        std::cout << ">> Register " << params[1] << " not found." << std::endl;
      return -1;
    }

    m_proc.set_reg_value(params[1], std::stoll(params[2], 0, 16));
    return 0;
  }else{
    return -1;
  }
  return -1;
}

int Memory::handle_command(std::vector<std::string> params){
  if (params.size() >= 2 && is_prefix(params[0], "read")){
    uint64_t addr;
    int reps = 1;

    try{
      addr = std::stoll(params[1], 0, 16);
    } catch (int e) {
      std::cout << ">> Address " << params[1] << " not a valid hexadecimal number." << std::endl;
      return -1;
    }

    if(params.size() > 2){
      try{
        reps = std::stol(params[2]);
      } catch (int e) {
        std::cout << ">> Repetition count " << params[2] << " not valid integer." << std::endl;
        return -1;
      }
    }

    for(int i = 0; i < reps; i++)
      std::cout << ">> " << int_to_hex(addr + i*8) << " : " << int_to_hex(m_proc.read_quad(addr + i*8)) << std::endl;

    return 0;
  }else if(params.size() == 3 && is_prefix(params[0], "write")){
    uint64_t addr;
    uint64_t data;
    try{
      addr = std::stoll(params[1], 0, 16);
    } catch (int e) {
      std::cout << ">> Address " << params[1] << " not a valid hexadecimal number." << std::endl;
      return -1;
    }

    try{
      data = std::stoll(params[2], 0, 16);
    } catch (int e) {
      std::cout << ">> Data " << params[1] << " not a valid hexadecimal number." << std::endl;
      return -1;
    }

    m_proc.write_quad(addr, data);
    std::cout << ">> " << params[1] << " : " << params[2] << std::endl;

    return 0;
  }else{
    return -1;
  }
}
