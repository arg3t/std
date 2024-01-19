#include "dwarf/dwarf++.hh"
#include <process.hxx>
#include <stdexcept>
#include <utils.hxx>

using namespace yeetdbg;

// TODO support arithmetic, switch to flex and bison

uint64_t search_string_in_dwarf(dwarf::die d, std::string input){

}

uint64_t search_string_in_die(dwarf::die d, std::string input){
}

uint64_t Process::resolve_input(std::string s){
  if(s.size() > 2 && s.substr(0, 2) == "0x"){
    return std::stoll(s.substr(2, s.size() - 2));
  } else if(s.size() > 1 && s.substr(0, 1) == "$"){
    return get_reg_value(s.substr(1, s.size() - 1));
  }

  try {
    return std::stoll(s);
  } catch (std::invalid_argument ex) { }

  try {
    return search_string_in_die();
  } catch (std::invalid_argument ex) { }


  throw std::invalid_argument(s + " cannot be resolved.");
}


dwarf::die Process::get_current_function(){
  uint64_t loc = negate_base(get_reg_value(reg::rip));

  for(auto &cu : m_dwarf.compilation_units()){
    if(!dwarf::die_pc_range(cu.root()).contains(loc))
      continue;

    for(auto& c : cu.root()){
      if(c.tag == dwarf::DW_TAG::subprogram)
        return c;
    }
  }

  throw std::out_of_range("RIP not in ELF range");
}
