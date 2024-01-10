#include <string>
#include <vector>

#ifndef UTILS_HXX
#define UTILS_HXX

std::vector<std::string> split(const std::string &s, char delimiter);

bool is_prefix(const std::string& s, const std::string& of);

std::string int_to_hex(long i);

#endif
