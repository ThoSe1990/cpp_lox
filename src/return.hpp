#pragma once 

#include <exception>
#include "lox_obj.hpp"

namespace cwt
{
  class lox_return : public std::exception
  {
    public:
      lox_return(const lox_obj& v);
      const char* what() const noexcept override;
      lox_obj value() const;
    private:
      lox_obj m_value;
  };
} // namespace cwt
