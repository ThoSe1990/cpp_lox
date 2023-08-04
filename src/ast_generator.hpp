
#include <iostream>
#include <variant>

namespace cwt
{   


template<typename Value>
struct expr_assign
{
  expr_assign(token op, const Value& value) : op(op), value(value) {}

  template<template<typename...> typename Visitor, typename T>
  T accept(const Visitor<T>& v) const
  {
    std::cout << "expr_assign accept" << std::endl;
    return v.visit(*this);
  }

  token op; 
  Value value;
};

template<typename Left, typename Right>
struct expr_binary
{
  expr_binary(const Left& left, token op, const Right& right) : left(left), op(op), right(right) {}

  template<template<typename...> typename Visitor, typename T>
  T accept(const Visitor<T>& v) const
  {
    std::cout << "expr_binary accept" << std::endl;
    return v.visit(*this);
  }
  Left left;
  token op;
  Right right;
};

template<typename Callee, typename Args> 
struct expr_call
{
  expr_call(const Callee& callee, token paren, std::vector<Args> args) : callee(callee), paren(paren), args(args) {}

  template<template<typename...> typename Visitor, typename T>
  T accept(const Visitor<T>& v) const
  {
    return v.visit(*this);
  }

  Callee callee;
  token paren;
  std::vector<Args> args; 

};

template<typename Obj> 
struct expr_get
{
  expr_get(const Obj& obj, token name) : obj(obj), name(name) {}

  template<template<typename...> typename Visitor, typename T>
  T accept(const Visitor<T>& v) const
  {
    return v.visit(*this);
  }

  Obj obj;
  token name; 
};

template<typename Expr>
struct expr_grouping
{
  expr_grouping(const Expr& expression) : expression(expression) {}
  
  template<template<typename...> typename Visitor, typename T>
  T accept(const Visitor<T>& v) const
  {
    return v.visit(*this);
  }

  Expr expression;
};

struct expr_literal
{
  expr_literal(const std::string& value) : value(value) {}

  template<template<typename...> typename Visitor, typename T>
  T accept(const Visitor<T>& v) const
  {
    return v.visit(*this);
  }

  std::string value;
};

template<typename Left, typename Right>
struct expr_logical
{
  expr_logical(const Left& left, token op, const Right& right) : left(left), op(op), right(right) {}

  template<template<typename...> typename Visitor, typename T>
  T accept(const Visitor<T>& v) const
  {
    return v.visit(*this);
  }

  Left left;
  token op;
  Right right;
};

template<typename Obj, typename Value>
struct expr_set 
{
  expr_set(const Obj& obj, token name, const Value& value) : obj(obj), name(name), value(value) {}

  template<template<typename...> typename Visitor, typename T>
  T accept(const Visitor<T>& v) const
  {
    return v.visit(*this);
  }

  Obj obj;
  token name; 
  Value value; 
};

struct expr_super
{
  expr_super(token keyword, token method) : keyword(keyword), method(method) {}

  template<template<typename...> typename Visitor, typename T>
  T accept(const Visitor<T>& v) const
  {
    return v.visit(*this);
  }

  token keyword;
  token method;
};

struct expr_this
{
  expr_this(token keyword) : keyword(keyword) {}

  template<template<typename...> typename Visitor, typename T>
  T accept(const Visitor<T>& v) const
  {
    return v.visit(*this);
  }

  token keyword;
};

template<typename Right>
struct expr_unary 
{
  expr_unary(token op, const Right& right) : op(op), right(right) {}

  template<template<typename...> typename Visitor, typename T>
  T accept(const Visitor<T>& v) const
  {
    return v.visit(*this);
  }

  token op;
  Right right; 
};

struct expr_variable 
{
  expr_variable(token name) : name(name) {}

  template<template<typename...> typename Visitor, typename T>
  T accept(const Visitor<T>& v) const
  {
    return v.visit(*this);
  }

  token name;
};


template<typename T>
class printer
{
  public:
    template<typename Expr>
    void print(Expr expr) const
    {
      std:: cout<< expr.accept(*this) << std::endl;
    }

    template<typename Value>
    T visit(const expr_assign<Value>& e) const 
    {
      return std::string("T visit(expr_assign<Value> e)");
    }

    template<typename Left, typename Right>
    T visit(const expr_binary<Left, Right>& e) const 
    {
      // return parenthesize(e.op.m_lexeme, e.left, e.right);
      return std::string("T visit(expr_binary<Value> e)");
    }

    template<typename Expr>
    T visit(const expr_grouping<Expr>& e) const 
    {
      return std::string("T visit(expr_grouping<Value> e)");
    }

    T visit(const expr_literal& e) const 
    {
      return std::to_string(e.value);
    }

    template<typename Right>
    T visit(const expr_unary<Right>& e) const 
    {
      return std::string("T visit(expr_grouping<Value> e)");
    }

  private:

    
    template<typename Expr>
    void parenthesize(Expr expr)
    {
      m_data.append(' ');
      m_data.append(expr.accept(*this));
    }

    template<typename... Exprs>
    T parenthesize(const std::string& name, Exprs... exprs)
    {
      m_data.append('(');
      m_data.append(name);
      parenthesize(exprs...);
      m_data.append(')');
      return m_data;
    }

    T m_data;
};

} // namespace cwt
