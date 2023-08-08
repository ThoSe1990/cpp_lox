#pragma once 

#include <string>
#include "token.hpp"

namespace cwt
{

  void runtime_error(const token& t, const std::string& msg);
  void report(const std::size_t line, const std::string& where, const std::string& msg);
  void error(const std::size_t line, const std::string& msg);

} // namespace cwt
