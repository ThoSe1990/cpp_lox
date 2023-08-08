#pragma once 

#include <vector>
#include <string> 

namespace cwt
{
  class lox_obj; 
  class interpreter; 
  
  struct lox_callable 
  {
    virtual ~lox_callable() = default;
    virtual std::size_t arity() = 0;
    virtual std::string to_string() = 0;
    virtual lox_obj call(interpreter& interpreter, const std::vector<lox_obj>& args) = 0;
  };

} // namespace cwt
