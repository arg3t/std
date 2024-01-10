#ifndef TABLEAU_HXX
#define TABLEAU_HXX

#include <cassert>
#include <breakpoint.hxx>
#include <sstream>
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

  std::string toString(bool b){
    return b ? "true": "false";
  }

  std::string toString(yeetdbg::Status s){
    switch(s){
      case ENABLED: return "Enabled";
      case DISABLED: return "Disabled";
      default: return "Unknown";
    }
  }

  std::string pad_str(const std::string& s, int size){
    auto so = std::string(s);
    int lpad = (size - s.size()) / 2;
    int rpad = (size - s.size()) / 2;

    if(lpad + rpad + s.size() < size)
      rpad++;

    assert(lpad > 0);
    assert(rpad > 0);

    so.insert(s.size(), rpad, ' ');
    so.insert(0, lpad, ' ');
    return so;
  }

  template <std::size_t I = 0, typename... Fields>
  typename std::enable_if_t<I == sizeof...(Fields), std::string>
  row_to_str(const std::tuple<Fields...> &dt, std::vector<int> sizes, char sep,
             int padding) {
    return std::string(1, sep);
  }

  template <std::size_t I = 0, typename... Fields>
  typename std::enable_if_t<(I < sizeof...(Fields)), std::string>
  row_to_str(const std::tuple<Fields...> &dt, std::vector<int> sizes, char sep,
             int padding) {
    auto s = std::string(1, sep);
    s += pad_str(toString(std::get<I>(dt)), sizes.at(I) + padding * 2);
    s += row_to_str<I+1>(dt, sizes, sep, padding);
    return s;
  }

  template<typename ...Fields>

  class Tableau{
    public:
      Tableau(std::vector<std::string> headers, char sep = '|', int padding = 2)
          : m_headers(headers), m_padding(padding), m_sep(sep),
            rows(std::vector<std::tuple<Fields ...>>()),
            max_sizes(std::vector<int>(sizeof...(Fields), 0)) {
        assert(m_headers.size() == sizeof...(Fields));

        int c = 0;
        for(auto& s: headers){
          max_sizes.at(c++) = s.size();
        }
      }

      void add_row(Fields &&...f) {
        auto t = std::tuple<Fields...>(f...);

        int c = 0;

        int unpack[]{0, (max_sizes.at(c++) = std::max(max_sizes.at(c), int(toString(f).length())) , 0)...};

        rows.push_back(t);
      }

      std::string gen_header_row(){
        std::string row = std::string();
        row += m_sep;

        for(int i = 0; i < m_headers.size(); i++){
          row += pad_str(m_headers.at(i), max_sizes.at(i) + m_padding * 2);
          if(i != m_headers.size() - 1)
            row += m_sep;
        }

        row += m_sep;

        return row;
      }

      std::vector<std::string> get_rows(){
        auto row_strings = std::vector<std::string>();

        for(auto &r : rows){
          row_strings.push_back(row_to_str(r, max_sizes, m_sep, m_padding));
        }

        return row_strings;
      }

      std::string to_string(){
        std::string os = std::string();

        os += gen_header_row() + std::string("\n");

        for(auto &i : get_rows()){
          os += i + std::string("\n");
        }

        return os;
      }

    private:
      std::vector<std::string> m_headers;
      std::vector<std::tuple<Fields...>> rows;
      std::vector<int> max_sizes;
      int m_padding;
      char m_sep;
  };
}

#endif
