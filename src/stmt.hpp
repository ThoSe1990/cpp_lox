#pragma once

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
    virtual void visit(const stmt_block<T>& s) const { throw std::runtime_error("stmt_visitor not implemented"); }
    virtual void visit(const stmt_class<T>& s) const { throw std::runtime_error("stmt_visitor not implemented"); }
    virtual void visit(const stmt_expression<T>& s) const { throw std::runtime_error("stmt_visitor not implemented"); }
    virtual void visit(const stmt_function<T>& s) const { throw std::runtime_error("stmt_visitor not implemented"); }
    virtual void visit(const stmt_if<T>& s) const { throw std::runtime_error("stmt_visitor not implemented"); }
    virtual void visit(const stmt_print<T>& s) const { throw std::runtime_error("stmt_visitor not implemented"); }
    virtual void visit(const stmt_return<T>& s) const { throw std::runtime_error("stmt_visitor not implemented"); }
    virtual void visit(const stmt_var<T>& s) const { throw std::runtime_error("stmt_visitor not implemented"); }
    virtual void visit(const stmt_while<T>& s) const { throw std::runtime_error("stmt_visitor not implemented"); }
  };

  template<typename T>
  struct statement
  {
    virtual ~statement() = default;
    virtual void accept(const stmt_visitor<T>& v) const = 0 ;
  };

  template<typename T>
  struct stmt_block : public statement<T>
  {
    using stmt_t = statement<T>;
    
    stmt_block(const std::vector<stmt_t*>& statements) : statements(statements) {}
    void accept(const stmt_visitor<T>& v) const override
    {
      return v.visit(*this);
    }
    std::vector<stmt_t*> statements;
  };

  template<typename T>
  struct stmt_class : public statement<T>
  {
    using stmt_t = statement<T>;
    using expr_t = expression<T>;
    using func_t = stmt_function<T>;

    stmt_class(token name, expr_t* superclass, const std::vector<func_t*>& methods) 
    : name(name), superclass(superclass), methods(methods) {}

    void accept(const stmt_visitor<T>& v) const override
    {
      return v.visit(*this);
    }

    token name;
    expr_t* superclass; 
    std::vector<func_t*> methods;
  };

  template<typename T>
  struct stmt_expression : public statement<T>
  {
    using stmt_t = statement<T>;
    using expr_t = expression<T>;

    stmt_expression(expr_t* expression) : expression(expression) {}

    void accept(const stmt_visitor<T>& v) const override
    {
      return v.visit(*this);
    }

    expr_t* expression;
  };

  template<typename T>
  struct stmt_function : public statement<T>
  {
    using stmt_t = statement<T>;
    
    stmt_function(token name, const std::vector<token>& parameters, const std::vector<stmt_t*>& body) 
    : name(name), parameters(parameters), body(body) {}
    
    void accept(const stmt_visitor<T>& v) const override
    {
      return v.visit(*this);
    }

    token name; 
    std::vector<token> parameters;
    std::vector<stmt_t*> body;
  };

  template<typename T>
  struct stmt_if : public statement<T>
  {
    using stmt_t = statement<T>;
    using expr_t = expression<T>;
    
    stmt_if(expr_t* condition, stmt_t* then_branch, stmt_t* else_branch) 
    : condition(condition), then_branch(then_branch), else_branch(else_branch) {}
    
    void accept(const stmt_visitor<T>& v) const override
    {
      return v.visit(*this);
    }

    expr_t* condition;
    stmt_t* then_branch;
    stmt_t* else_branch;
  };

  template<typename T>
  struct stmt_print : public statement<T>
  {
    using stmt_t = statement<T>;
    using expr_t = expression<T>;
    
    stmt_print(expr_t* expression) : expression(expression) {}

    void accept(const stmt_visitor<T>& v) const override
    {
      return v.visit(*this);
    }

    expr_t* expression;
  };

  template<typename T>
  struct stmt_return : public statement<T>
  {
    using stmt_t = statement<T>;
    using expr_t = expression<T>;
  
    stmt_return(token keyword, expr_t* value) 
    : keyword(keyword), value(value) {}
    
    void accept(const stmt_visitor<T>& v) const override
    {
      return v.visit(*this);
    }

    token keyword; 
    expr_t* value;

  };
  template<typename T>
  struct stmt_var : public statement<T>
  {
    using stmt_t = statement<T>;
    using expr_t = expression<T>;

    stmt_var(token name, expr_t* initializer) 
    : name(name), initializer(initializer) {}
    
    void accept(const stmt_visitor<T>& v) const override
    {
      return v.visit(*this);
    }
  
    token name; 
    expr_t* initializer;
  };

  template<typename T>
  struct stmt_while : public statement<T>
  {
    using stmt_t = statement<T>;
    using expr_t = expression<T>;

    stmt_while(expr_t* condition, stmt_t* body) 
    : condition(condition), body(body) {}
    
    void accept(const stmt_visitor<T>& v) const override
    {
      return v.visit(*this);
    }

    expr_t* condition;
    stmt_t* body;
  };


}