#pragma once 

#include "lox_callable.hpp"

namespace cwt
{

template<typename T>
struct stmt_function;

class lox_function : public lox_callable
{
  public:
    lox_function(const stmt_function<lox_obj>* declaration);
    std::string to_string() override;
    std::size_t arity() override;
    lox_obj call(interpreter& interpreter, const std::vector<lox_obj>& args) override;

  private: 
    const stmt_function<lox_obj>* m_declaration;
};


} // namespace cwt
