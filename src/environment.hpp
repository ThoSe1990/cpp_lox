#pragma once

#include <string>
#include <unordered_map>

#include "token.hpp"
#include "lox_obj.hpp"

namespace cwt
{
  class environment
  {
    public:
      void set_enclosing(environment* env) ;
      void define(const std::string& name, const lox_obj& value);
      void assign(token name, const lox_obj& value);
      lox_obj& get(token t);
    private:
      std::unordered_map<std::string, lox_obj> m_data;
      environment* m_enclosing;
  };
} // namespace cwt
