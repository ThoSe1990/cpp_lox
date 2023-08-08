
#include <iostream>
#include <memory>

#include "interpreter.hpp"
#include "lox_function.hpp"
#include "error.hpp"
#include "return.hpp"

namespace cwt
{
environment* interpreter::get_env_ptr()
{
  return m_env.get();
}
void interpreter::interpret(const std::vector<stmt_t>& statements) 
{
  try
  {
    this->execute(statements);
  }
  catch(const std::exception& e)
  {
    std::cerr << e.what() << '\n';
  }
}

void interpreter::visit(const stmt_block<lox_obj>& s)  
{
  execute_block(s.statements, std::make_unique<environment>());
}
void interpreter::visit(const stmt_expression<lox_obj>& s)  
{
  evaluate(s.expression);
}
void interpreter::visit(const stmt_function<lox_obj>& s)
{
  lox_function f(&s);
  lox_obj obj(f);
  m_env->define(s.name.lexeme, create_another(obj));
}
void interpreter::visit(const stmt_if<lox_obj>& s) 
{
  if (is_truthy(evaluate(s.condition)))
  {
    execute(s.then_branch);
  }
  else if (!s.else_branch.empty()) 
  {
    execute(s.else_branch);
  }
}
void interpreter::visit(const stmt_print<lox_obj>& s)  
{
  lox_obj value = evaluate(s.expression);
  std::cout << value.to_string() << std::endl;
}
void interpreter::visit(const stmt_var<lox_obj>& s) 
{
  lox_obj value;
  if (s.initializer)
  {
    value = evaluate(s.initializer);
  }
  m_env->define(s.name.lexeme, value);
}
void interpreter::visit(const stmt_while<lox_obj>& s) 
{
  while (is_truthy(evaluate(s.condition)))
  {
    execute(s.body);
  }
}
void interpreter::visit(const stmt_return<lox_obj>& s)
{
  lox_obj value;
  if (s.value) 
  {
    value = evaluate(s.value);
  }
  throw lox_return(create_another(value));
}

lox_obj interpreter::visit(const expr_assign<lox_obj>& e)
{
  lox_obj value = evaluate(e.value);
  m_env->assign(e.name, value);
  return create_another(value);
}

lox_obj interpreter::visit(const expr_literal<lox_obj>& e) 
{
  return create_another(e.value);
}

lox_obj interpreter::visit(const expr_logical<lox_obj>& e)  
{
  lox_obj left = evaluate(e.left);
  if (e.op.type == token_type::OR)
  {
    if (is_truthy(left)) { return left; }
  }
  else 
  {
    if (!is_truthy(left)) { return left; }
  }
  return evaluate(e.right);
}

lox_obj interpreter::visit(const expr_grouping<lox_obj>& e)
{
  return evaluate(e.expr);
}

lox_obj interpreter::visit(const expr_unary<lox_obj>& e)
{
  lox_obj right = evaluate(e.right);
  switch (e.op.type)
  {
    case token_type::BANG: return !is_truthy(right);
    break;case token_type::MINUS: 
    {
      check_number_operand(e.op, right);
      return lox_obj(-1*right.number());
    }
  }
  return lox_obj(); // equivalent to null
}

lox_obj interpreter::visit(const expr_variable<lox_obj>& e)
{
  return create_another(m_env->get(e.name));
}

lox_obj interpreter::visit(const expr_binary<lox_obj>& e) 
{
  lox_obj left = evaluate(e.left);
  lox_obj right = evaluate(e.right);
  switch (e.op.type)
  {
    case token_type::GREATER : 
      check_number_operand(e.op, left, right);
      return left.number() >  right.number();
    break; case token_type::GREATER_EQUAL : 
      check_number_operand(e.op, left, right);
      return left.number() >=  right.number();
    break; case token_type::LESS : 
      check_number_operand(e.op, left, right);
      return left.number() <  right.number();
    break; case token_type::LESS_EQUAL :
      check_number_operand(e.op, left, right);
      return left.number() <=  right.number();
    break; case token_type::BANG_EQUAL : return !is_equal(left, right);
    break; case token_type::EQUAL_EQUAL : return is_equal(left, right);
    break; case token_type::MINUS : 
      check_number_operand(e.op, left, right);
      return left.number() - right.number();
    break; case token_type::SLASH : 
      check_number_operand(e.op, left, right);
      return left.number() / right.number();
    break; case token_type::STAR : 
      check_number_operand(e.op, left, right);
      return left.number() * right.number();
    break; case token_type::PLUS :
    {
      if (left.type() == value_type::number && right.type() == value_type::number)
      {
        return left.number() + right.number();
      }
      else if (left.type() == value_type::string && right.type() == value_type::string)
      {
        std::string s = left.string();
        s.append(right.string());
        return s;
      }
      else 
      {
        runtime_error(e.op, " Operands must be two numbers or two strings.");
      }
    }
  }
}


lox_obj interpreter::visit(const expr_call<lox_obj>& e)
{
  lox_obj callee = evaluate(e.callee);
  std::vector<lox_obj> args;
  for (const expr_t& arg : e.args) 
  {
    args.push_back(evaluate(arg));
  }

  if (callee.type() == value_type::callable) 
  {
    lox_function func = callee.callable();
    if (args.size() != func.arity()) 
    { 
      std::string s{"Expected "};
      s.append(std::to_string(func.arity()));
      s.append(" arguments but got ");
      s.append(std::to_string(args.size()));
      s.append(".");
      runtime_error(e.paren, s);
    }
    else
    {
      return func.call(*this, args);
    }
  } 
  else 
  {
    runtime_error(e.paren, "Can only call functions and classes.");
  }
  throw std::runtime_error("fick dich");
}

void interpreter::execute(const stmt_t& statement)
{
  statement->accept(*this);
}
void interpreter::execute(const std::vector<stmt_t>& statements)
{
  for (const auto& s : statements)
  {
    s->accept(*this);
  }
}

void interpreter::execute_block(const std::vector<stmt_t>& statements, std::unique_ptr<environment> new_env)
{
  std::unique_ptr<environment> prev = std::move(m_env);
  try
  {
    finally on_exit([this, &prev]()
    { 
      m_env = std::move(prev); 
    });

    m_env = std::move(new_env);
    m_env->set_enclosing(prev.get());
    execute(statements);
  }
  catch(const lox_return& e)
  {
    throw lox_return(e.value());
  } 
  catch(const std::exception& e)
  {
    std::cerr << e.what() << '\n';
  }

}
      

lox_obj interpreter::evaluate(const expr_t& e)  
{
  return e->accept(*this);
}
bool interpreter::is_truthy(const lox_obj& obj)  
{
  if (obj.nil())
  {
    return false;
  }
  else if (obj.type() == value_type::boolean) 
  {
    return obj.boolean();
  }
  else 
  {
    return true;
  }
}
bool interpreter::is_equal(const lox_obj& left, const lox_obj& right) const 
{
  auto both_type = [&left, &right](value_type t) { return left.type() == t && right.type() == t; };

  if (left.nil() && right.nil()) 
  {
    return true;
  }
  else if (both_type(value_type::number))
  {
    return left.number() == right.number();
  }
  else if (both_type(value_type::boolean))
  {
    return left.boolean() == right.boolean();
  }
  else if (both_type(value_type::string))
  {
    return left.string() == right.string();
  }
  else 
  {
    return false; 
  }
}

void interpreter::check_number_operand(const token& op, const lox_obj& operand) const 
{
  if (operand.type() == value_type::number) 
  {
    return;
  }
  else 
  {
    runtime_error(op, " Operand must be a number.");
  }
}
void interpreter::check_number_operand(const token& op, const lox_obj& left, const lox_obj& right) const 
{
  if (left.type() == value_type::number && right.type() == value_type::number) 
  {
    return;
  }
  else 
  {
    runtime_error(op, " Operands must be a number.");
  }
}

} // namespace cwt
