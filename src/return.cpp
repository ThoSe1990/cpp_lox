#pragma once 

#include "return.hpp"

namespace cwt
{

  lox_return::lox_return(const lox_obj& v) : m_value(create_another(v)) {}
  
  const char* lox_return::what() const noexcept 
  {
    return "...";
  }
  
  lox_obj lox_return::value() const
  {
    return create_another(m_value);
  }

} // namespace cwt
