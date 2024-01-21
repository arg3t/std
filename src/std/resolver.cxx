#include "dwarf/dwarf++.hh"
#include <process.hxx>
#include <stdexcept>
#include <string>
#include <utils.hxx>
#include <fstream>
#include <vector>
#include <format>

using namespace std;

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

  /* try { */
  /*   return search_string_in_die(); */
  /* } catch (std::invalid_argument ex) { } */

  throw std::invalid_argument(s + " cannot be resolved.");
}

dwarf::die Process::get_die_at_addr(uint64_t addr){
  for(auto cu : m_dwarf.compilation_units()){
    if(!dwarf::die_pc_range(cu.root()).contains(addr))
      continue;

    for(auto& c : cu.root()){
      if(c.tag == dwarf::DW_TAG::subprogram)
        return c;
    }
  }

  throw std::out_of_range("RIP not in ELF range");
}

dwarf::line_table::iterator Process::get_line_table_entry(uint64_t addr){
  for(auto &cu : m_dwarf.compilation_units()){
    if(!dwarf::die_pc_range(cu.root()).contains(addr))
      continue;

    auto lt = cu.get_line_table();
    return lt.find_address(addr);
  }

  throw std::out_of_range("addr not in ELF range");
}

std::vector<std::string> Process::get_src_for_address(uint64_t addr, int surround_line_count){
  auto e = get_line_table_entry(negate_base(addr));
  std::ifstream file(e->file->path);
  std::vector<std::string> lines;

  std::string l;
  int count = 0;

  while(std::getline(file, l)){
    if (count > e->line - surround_line_count &&
        count < e->line + surround_line_count) {
      if(count == e->line)
        lines.push_back(std::format("[{}]: {}", e->line + count, l));
      else
        lines.push_back(std::format("{}: {}", e->line + count, l));
    }

    if(count >= e->line + surround_line_count)
      break;
  }

  return lines;
}


dwarf::die Process::get_current_function(){
  return get_die_at_addr(negate_base(get_reg_value(reg::rip)));
}
