#pragma once

#include "expr.hpp"

namespace cwt
{   
  template<typename T> struct stmt_block;
  template<typename T> struct stmt_class;
  template<typename T> struct stmt_expression;
  template<typename T> struct stmt_function;
  template<typename T> struct stmt_if;
  template<typename T> struct stmt_print;
  template<typename T> struct stmt_return;
  template<typename T> struct stmt_var;
  template<typename T> struct stmt_while;

  template<typename T>
  struct stmt_visitor 
  {
    virtual ~stmt_visitor() = default;
    virtual void visit(const stmt_block<T>& s) { throw std::runtime_error("stmt_visitor not implemented"); }
    virtual void visit(const stmt_class<T>& s) { throw std::runtime_error("stmt_visitor not implemented"); }
    virtual void visit(const stmt_expression<T>& s) { throw std::runtime_error("stmt_visitor not implemented"); }
    virtual void visit(const stmt_function<T>& s) { throw std::runtime_error("stmt_visitor not implemented"); }
    virtual void visit(const stmt_if<T>& s) { throw std::runtime_error("stmt_visitor not implemented"); }
    virtual void visit(const stmt_print<T>& s) { throw std::runtime_error("stmt_visitor not implemented"); }
    virtual void visit(const stmt_return<T>& s) { throw std::runtime_error("stmt_visitor not implemented"); }
    virtual void visit(const stmt_var<T>& s) { throw std::runtime_error("stmt_visitor not implemented"); }
    virtual void visit(const stmt_while<T>& s) { throw std::runtime_error("stmt_visitor not implemented"); }
  };

  template<typename T>
  struct lox_statement
  {
    virtual ~lox_statement() = default;
    virtual void accept(stmt_visitor<T>& v) = 0;
  };

  template<typename T>
  struct stmt_block : public lox_statement<T>
  {
    using stmt_t = std::unique_ptr<lox_statement<T>>;
    
    stmt_block(std::vector<stmt_t> statements) : statements(std::move(statements)) {}
    void accept(stmt_visitor<T>& v) override
    {
      return v.visit(*this);
    }
    std::vector<stmt_t> statements;
  };

  template<typename T>
  struct stmt_class : public lox_statement<T>
  {
    using stmt_t = std::unique_ptr<lox_statement<T>>;
    using expr_t = std::unique_ptr<lox_expression<T>>;
    using func_t = stmt_function<T>;

    stmt_class(token name, expr_t superclass, const std::vector<func_t*>& methods) 
    : name(name), superclass(std::move(superclass)), methods(std::move(methods)) {}

    void accept(stmt_visitor<T>& v) override
    {
      return v.visit(*this);
    }

    token name;
    expr_t superclass; 
    std::vector<func_t*> methods;
  };

  template<typename T>
  struct stmt_expression : public lox_statement<T>
  {
    using stmt_t = std::unique_ptr<lox_statement<T>>;
    using expr_t = std::unique_ptr<lox_expression<T>>;

    stmt_expression(expr_t expression) : expression(std::move(expression)) {}

    void accept( stmt_visitor<T>& v) override
    {
      return v.visit(*this);
    }

    expr_t expression;
  };

  template<typename T>
  struct stmt_function : public lox_statement<T>
  {
    using stmt_t = std::unique_ptr<lox_statement<T>>;
    
    stmt_function(token name, const std::vector<token>& parameters, std::vector<stmt_t> body) 
    : name(name), parameters(parameters), body(std::move(body)) {}
    
    void accept( stmt_visitor<T>& v) override
    {
      return v.visit(*this);
    }

    token name; 
    std::vector<token> parameters;
    std::vector<stmt_t> body;
  };

  template<typename T>
  struct stmt_if : public lox_statement<T>
  {
    using stmt_t = std::unique_ptr<lox_statement<T>>;
    using expr_t = std::unique_ptr<lox_expression<T>>;
    
    stmt_if(expr_t condition, std::vector<stmt_t> then_branch, std::vector<stmt_t> else_branch) 
    : condition(std::move(condition)), then_branch(std::move(then_branch)), else_branch(std::move(else_branch)) {}
    
    void accept( stmt_visitor<T>& v) override
    {
      return v.visit(*this);
    }

    expr_t condition;
    std::vector<stmt_t> then_branch;
    std::vector<stmt_t> else_branch;
  };

  template<typename T>
  struct stmt_print : public lox_statement<T>
  {
    using stmt_t = std::unique_ptr<lox_statement<T>>;
    using expr_t = std::unique_ptr<lox_expression<T>>;
    
    stmt_print(expr_t expression) : expression(std::move(expression)) {}

    void accept( stmt_visitor<T>& v) override
    {
      return v.visit(*this);
    }

    expr_t expression;
  };

  template<typename T>
  struct stmt_return : public lox_statement<T>
  {
    using stmt_t = std::unique_ptr<lox_statement<T>>;
    using expr_t = std::unique_ptr<lox_expression<T>>;
  
    stmt_return(token keyword, expr_t value) 
    : keyword(keyword), value(value) {}
    
    void accept( stmt_visitor<T>& v) override
    {
      return v.visit(*this);
    }

    token keyword; 
    expr_t value;

  };
  template<typename T>
  struct stmt_var : public lox_statement<T>
  {
    using stmt_t = std::unique_ptr<lox_statement<T>>;
    using expr_t = std::unique_ptr<lox_expression<T>>;

    stmt_var(token name, expr_t initializer) 
    : name(name), initializer(std::move(initializer)) {}
    
    void accept( stmt_visitor<T>& v) override
    {
      return v.visit(*this);
    }
  
    token name; 
    expr_t initializer;
  };

  template<typename T>
  struct stmt_while : public lox_statement<T>
  {
    using stmt_t = std::unique_ptr<lox_statement<T>>;
    using expr_t = std::unique_ptr<lox_expression<T>>;

    stmt_while(expr_t condition, std::vector<stmt_t> body) 
    : condition(std::move(condition)), body(std::move(body)) {}
    
    void accept( stmt_visitor<T>& v) override
    {
      return v.visit(*this);
    }

    expr_t condition;
    std::vector<stmt_t> body;
  };


}