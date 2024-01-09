#include <iomanip>
#include <sstream>

#include "utils.hxx"

std::vector<std::string> split(const std::string &s, char delimiter){
  std::vector<std::string> out {};
  std::stringstream ss {s};

  std::string item;

  while(std::getline(ss, item, delimiter)) {
    out.push_back(item);
  }

  return out;
}

bool is_prefix(const std::string& s, const std::string& of){
  if (s.size() > of.size()) return false;

  return std::equal(s.begin(), s.end(), of.begin());
}

std::string int_to_hex(long i){
  std::stringstream stream;
  stream << "0x"
         << std::setfill ('0') << std::setw(sizeof(long)*2)
         << std::hex << i;
  return stream.str();
}
