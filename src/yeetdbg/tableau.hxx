#ifndef TABLEAU_HXX
#define TABLEAU_HXX

#include <cassert>
#include <string>
#include <tuple>
#include <vector>

namespace yeetdbg {

  std::string toString(std::string s){
    return s;
  }

  std::string toString(char* s){
    return s;
  }

  template <typename T>

  std::string toString(T& s){
    return std::to_string(s);
  }

  template<typename ...Fields>

  class Tableau{
    public:
      Tableau(std::vector<std::string> headers)
          : m_headers(headers), rows(std::vector<std::tuple<Fields&...>>()),
            max_sizes(std::vector<int>(sizeof...(Fields), 0)) {
        assert(m_headers.size() == sizeof...(Fields));
      }

      void add_row(Fields &&...f) {
        auto t = std::tuple<Fields&...>(f...);

        int c = 0;

        int unpack[]{0, (max_sizes.at(c++) = std::max(max_sizes.at(c), int(toString(f).length())) , 0)...};

        rows.push_back(t);
      }

      void print(Fields &&...f) {

      }
    private:
      std::vector<std::string> m_headers;
      std::vector<std::tuple<Fields&...>> rows;
      std::vector<int> max_sizes;
  };
}


std::ostream& operator<<(std::ostream& os, const yeetdbg::Tableau& dt)
{
    os << dt.mo << '/' << dt.da << '/' << dt.yr;
    return os;
}

#endif
