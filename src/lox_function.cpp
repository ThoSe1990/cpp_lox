#include "lox_function.hpp"
#include "lox_obj.hpp"
#include "environment.hpp"
#include "interpreter.hpp"

namespace cwt
{

lox_function::lox_function(const stmt_function<lox_obj>* declaration) : m_declaration(declaration)
{

}
std::string lox_function::to_string()
{
  return "some function lol";
}
std::size_t lox_function::arity()
{
  return m_declaration->parameters.size();
}
lox_obj lox_function::call(interpreter& interpreter, const std::vector<lox_obj>& args)
{
  auto env = std::make_unique<environment>();
  env->set_enclosing(interpreter.get_env_ptr());
  for (std::size_t i = 0 ; i < m_declaration->parameters.size() ; ++i)
  {
    env->define(m_declaration->parameters[i].lexeme, args.at(i));
  }
  interpreter.execute_block(m_declaration->body, std::move(env));
  return lox_obj{};
}

} // namespace cwt
