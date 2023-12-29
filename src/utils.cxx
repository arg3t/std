#include <sstream>

#include "utils.hxx"

std::vector<std::string> utils::split(const std::string &s, char delimiter){
  std::vector<std::string> out {};
  std::stringstream ss {s};

  std::string item;

  while(std::getline(ss, item, delimiter)) {
    out.push_back(item);
  }

  return out;
}

bool utils::is_prefix(const std::string& s, const std::string& of){
  if (s.size() > of.size()) return false;

  return std::equal(s.begin(), s.end(), of.begin());
}
