#pragma once

#include "token.hpp"
#include "lox_obj.hpp"


namespace cwt
{

  template<typename T> struct expr_assign;
  template<typename T> struct expr_binary;
  template<typename T> struct expr_call;
  template<typename T> struct expr_get;
  template<typename T> struct expr_grouping;
  template<typename T> struct expr_literal;
  template<typename T> struct expr_logical;
  template<typename T> struct expr_set;
  template<typename T> struct expr_super;
  template<typename T> struct expr_this;
  template<typename T> struct expr_unary;
  template<typename T> struct expr_variable;

  template<typename T>
  struct expr_visitor 
  {
    virtual ~expr_visitor() = default;
    virtual T visit(const expr_assign<T>& e) { throw std::runtime_error("expr_visitor not implemented"); }
    virtual T visit(const expr_binary<T>& e) { throw std::runtime_error("expr_visitor not implemented"); }
    virtual T visit(const expr_call<T>& e) { throw std::runtime_error("expr_visitor not implemented"); }
    virtual T visit(const expr_get<T>& e) { throw std::runtime_error("expr_visitor not implemented"); }
    virtual T visit(const expr_grouping<T>& e) { throw std::runtime_error("expr_visitor not implemented"); }
    virtual T visit(const expr_literal<T>& e) { throw std::runtime_error("expr_visitor not implemented"); }
    virtual T visit(const expr_logical<T>& e) { throw std::runtime_error("expr_visitor not implemented"); }
    virtual T visit(const expr_set<T>& e) { throw std::runtime_error("expr_visitor not implemented"); }
    virtual T visit(const expr_super<T>& e) { throw std::runtime_error("expr_visitor not implemented"); }
    virtual T visit(const expr_this<T>& e) { throw std::runtime_error("expr_visitor not implemented"); }
    virtual T visit(const expr_unary<T>& e) { throw std::runtime_error("expr_visitor not implemented"); }
    virtual T visit(const expr_variable<T>& e) { throw std::runtime_error("expr_visitor not implemented"); }
  };

  enum class expr_type
  {
    _assign = 0, _binary, _call, _get, _grouping, _literal, _logical, _set, _super, _this, _unary, _variable
  };

  template<typename T>
  struct lox_expression
  {
    virtual ~lox_expression() = default;
    virtual T accept(expr_visitor<T>& v) = 0 ;
    virtual expr_type type() = 0;
  };

  template<typename T> 
  struct expr_assign : public lox_expression<T>
  {
    using expr_t = std::unique_ptr<lox_expression<T>>;
    expr_assign(token name, expr_t value) : name(name), value(std::move(value)) {}

    expr_type type() { return expr_type::_assign; };
    T accept(expr_visitor<T>& v) override
    {
      return v.visit(*this);
    } 

    token name;
    expr_t value;
  };

  template<typename T>
  struct expr_binary : public lox_expression<T>
  {
    using expr_t = std::unique_ptr<lox_expression<T>>;
    expr_binary(expr_t left, token op, expr_t right) : left(std::move(left)), op(op), right(std::move(right)) {}

    expr_type type() { return expr_type::_binary; };
    T accept(expr_visitor<T>& v) override
    {
      return v.visit(*this);
    }
    expr_t left;
    token op;
    expr_t right;
  };

  template<typename T>
  struct expr_call : public lox_expression<T>
  {
    using expr_t = std::unique_ptr<lox_expression<T>>;
    expr_call(expr_t callee, token paren, std::vector<expr_t> args) : callee(std::move(callee)), paren(paren), args(std::move(args)) {}

    expr_type type() { return expr_type::_call; };
    T accept(expr_visitor<T>& v) override
    {
      return v.visit(*this);
    }

    expr_t callee;
    token paren;
    std::vector<expr_t> args; 
  };

  template<typename T>
  struct expr_get : public lox_expression<T>
  {
    using expr_t = std::unique_ptr<lox_expression<T>>;
    expr_get(expr_t obj, token name) : obj(std::move(obj)), name(name) {}

    expr_type type() { return expr_type::_get; };
    T accept(expr_visitor<T>& v) override
    {
      return v.visit(*this);
    }

    expr_t obj;
    token name; 
  };

  template<typename T>
  struct expr_grouping : public lox_expression<T>
  {
    using expr_t = std::unique_ptr<lox_expression<T>>;
    expr_grouping(expr_t expr) : expr(std::move(expr)) {}

    expr_type type() { return expr_type::_grouping; };
    T accept(expr_visitor<T>& v) override
    {
      return v.visit(*this);
    }

    expr_t expr;
  };

  template<typename T>
  struct expr_literal : public lox_expression<T> 
  {
    expr_literal(bool v) : value(v) {}
    expr_literal(double v) : value(v) {}
    expr_literal(std::string v) : value(v) {}
    expr_literal() : value(lox_obj()) {}

    expr_type type() { return expr_type::_literal; };
    T accept(expr_visitor<T>& v) override
    {
      return v.visit(*this);
    }
    
    T value;
  };

  template<typename T>
  struct expr_logical : public lox_expression<T>
  {
    using expr_t = std::unique_ptr<lox_expression<T>>;
    expr_logical(expr_t left, token op, expr_t right) : left(std::move(left)), op(op), right(std::move(right)) {}

    expr_type type() { return expr_type::_logical; };
    T accept(expr_visitor<T>& v) override
    {
      return v.visit(*this);
    }

    expr_t left;
    token op;
    expr_t right;
  };

  template<typename T>
  struct expr_set : public lox_expression<T> 
  {
    using expr_t = std::unique_ptr<lox_expression<T>>;
    expr_set(expr_t obj, token name, expr_t value) : obj(std::move(obj)), name(name), value(std::move(value)) {}

    expr_type type() { return expr_type::_set; };
    T accept(expr_visitor<T>& v) override
    {
      return v.visit(*this);
    }

    expr_t obj;
    token name; 
    expr_t value; 
  };

  template<typename T>
  struct expr_super : public lox_expression<T> 
  {
    using expr_t = std::unique_ptr<lox_expression<T>>;
    expr_super(token keyword, token method) : keyword(keyword), method(method) {}

    expr_type type() { return expr_type::_super; };
    T accept(expr_visitor<T>& v) override
    {
      return v.visit(*this);
    }

    token keyword;
    token method;
  };

  template<typename T>
  struct expr_this : public lox_expression<T> 
  {
    using expr_t = std::unique_ptr<lox_expression<T>>;
    expr_this(token keyword) : keyword(keyword) {}

    expr_type type() { return expr_type::_this; };
    T accept(expr_visitor<T>& v) override
    {
      return v.visit(*this);
    }

    token keyword;
  };

  template<typename T>
  struct expr_unary : public lox_expression<T> 
  {
    using expr_t = std::unique_ptr<lox_expression<T>>;
    expr_unary(token op, expr_t right) : op(op), right(std::move(right)) {}

    expr_type type() { return expr_type::_unary; };
    T accept(expr_visitor<T>& v) override
    {
      return v.visit(*this);
    }

    token op;
    expr_t right; 
  };

  template<typename T>
  struct expr_variable : public lox_expression<T> 
  {
    using expr_t = std::unique_ptr<lox_expression<T>>;
    using underlying_t = T;
    expr_variable(token name) : name(name) {}
    expr_type type() { return expr_type::_variable; };
    T accept(expr_visitor<T>& v) override
    {
      return v.visit(*this);
    }

    token name;
  };

} // namespace cwt
