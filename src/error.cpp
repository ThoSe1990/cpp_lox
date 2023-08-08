#include <stdexcept>
#include <iostream> 

#include "error.hpp"

namespace cwt 
{
  bool has_error = false;
  bool has_runtime_error = false; 

  void runtime_error(const token& t, const std::string& msg)
  {
    std::string s{t.to_string()};
    s.append(" Operands must be two numbers or two strings.");
    has_runtime_error = true;
    throw std::runtime_error(s);
  }

  void report(const std::size_t line, const std::string& where, const std::string& msg)
  {
    std::cerr << "[REPORT] " << where << ':' << line << ": " << msg << '\n';
    has_error = true;
  }

  void error(const std::size_t line, const std::string& msg) 
  {
    report(line, "", msg);
  }

} // namespace cwt
