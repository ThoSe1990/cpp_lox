#pragma once

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
    virtual T visit(const expr_assign<T>& e) const { throw std::runtime_error("expr_visitor not implemented"); }
    virtual T visit(const expr_binary<T>& e) const { throw std::runtime_error("expr_visitor not implemented"); }
    virtual T visit(const expr_call<T>& e) const { throw std::runtime_error("expr_visitor not implemented"); }
    virtual T visit(const expr_get<T>& e) const { throw std::runtime_error("expr_visitor not implemented"); }
    virtual T visit(const expr_grouping<T>& e) const { throw std::runtime_error("expr_visitor not implemented"); }
    virtual T visit(const expr_literal<T>& e) const { throw std::runtime_error("expr_visitor not implemented"); }
    virtual T visit(const expr_logical<T>& e) const { throw std::runtime_error("expr_visitor not implemented"); }
    virtual T visit(const expr_set<T>& e) const { throw std::runtime_error("expr_visitor not implemented"); }
    virtual T visit(const expr_super<T>& e) const { throw std::runtime_error("expr_visitor not implemented"); }
    virtual T visit(const expr_this<T>& e) const { throw std::runtime_error("expr_visitor not implemented"); }
    virtual T visit(const expr_unary<T>& e) const { throw std::runtime_error("expr_visitor not implemented"); }
    virtual T visit(const expr_variable<T>& e) const { throw std::runtime_error("expr_visitor not implemented"); }
  };

  template<typename T>
  struct expression
  {
    virtual ~expression() = default;
    virtual T accept(const expr_visitor<T>& v) const = 0 ;
  };

  template<typename T> 
  struct expr_assign : public expression<T>
  {
    using expr_t = expression<T>;
    expr_assign(token op, expr_t* value) : op(op), value(value) {}
    ~expr_assign()  
    {
      if (value) {delete value;}
    }
    T accept(const expr_visitor<T>& v) const override
    {
      return v.visit(*this);
    } 

    token op;
    expr_t* value;
  };

  template<typename T>
  struct expr_binary : public expression<T>
  {
    using expr_t = expression<T>;
    expr_binary(expr_t* left, token op, expr_t* right) : left(left), op(op), right(right) {}
    ~expr_binary() 
    {
      if (left) {delete left;}
      if (right) {delete right;}
    }
    T accept(const expr_visitor<T>& v) const override
    {
      return v.visit(*this);
    }
    expr_t* left;
    token op;
    expr_t* right;
  };

  template<typename T>
  struct expr_call : public expression<T>
  {
    using expr_t = expression<T>;
    expr_call(expr_t* callee, token paren, const std::vector<expr_t*>& args) : callee(callee), paren(paren), args(args) {}
  	~expr_call()
    {
      if (callee) {delete callee;}
      for (auto e : args) {if (e) delete e;}
    }
    T accept(const expr_visitor<T>& v) const override
    {
      return v.visit(*this);
    }

    expr_t* callee;
    token paren;
    std::vector<expr_t*> args; 
  };

  template<typename T>
  struct expr_get : public expression<T>
  {
    using expr_t = expression<T>;
    expr_get(expr_t* obj, token name) : obj(obj), name(name) {}
    ~expr_get()
    {
      if (obj) { delete obj; }
    }
    T accept(const expr_visitor<T>& v) const override
    {
      return v.visit(*this);
    }

    expr_t* obj;
    token name; 
  };

  template<typename T>
  struct expr_grouping : public expression<T>
  {
    using expr_t = expression<T>;
    expr_grouping(expr_t* expr) : expr(expr) {}
    ~expr_grouping()  
    {
      if (expr) {delete expr;}
    }
    T accept(const expr_visitor<T>& v) const override
    {
      return v.visit(*this);
    }

    expr_t* expr;
  };

  template<typename T>
  struct expr_literal : public expression<T> 
  {
    expr_literal(bool v) : value(v) {}
    expr_literal(double v) : value(v) {}
    expr_literal(std::string v) : value(v) {}
    expr_literal() : value(lox_obj()) {}

    ~expr_literal() {}

    T accept(const expr_visitor<T>& v) const override
    {
      return v.visit(*this);
    }
    
    lox_obj value;
  };

  template<typename T>
  struct expr_logical : public expression<T>
  {
    using expr_t = expression<T>;
    expr_logical(expr_t* left, token op, expr_t* right) : left(left), op(op), right(right) {}
    ~expr_logical() 
    {
      if (left) {delete left;}
      if (right) {delete right;}
    }
    T accept(const expr_visitor<T>& v) const override
    {
      return v.visit(*this);
    }

    expr_t* left;
    token op;
    expr_t* right;
  };

  template<typename T>
  struct expr_set : public expression<T> 
  {
    using expr_t = expression<T>;
    expr_set(expr_t* obj, token name, expr_t* value) : obj(obj), name(name), value(value) {}
    ~expr_set() 
    {
      if (obj) { delete obj; }
      if (value) { delete value; }
    }
    T accept(const expr_visitor<T>& v) const override
    {
      return v.visit(*this);
    }

    expr_t* obj;
    token name; 
    expr_t* value; 
  };

  template<typename T>
  struct expr_super : public expression<T> 
  {
    using expr_t = expression<T>;
    expr_super(token keyword, token method) : keyword(keyword), method(method) {}
    ~expr_super() = default;
    T accept(const expr_visitor<T>& v) const override
    {
      return v.visit(*this);
    }

    token keyword;
    token method;
  };

  template<typename T>
  struct expr_this : public expression<T> 
  {
    using expr_t = expression<T>;
    expr_this(token keyword) : keyword(keyword) {}
    ~expr_this() = default;

    T accept(const expr_visitor<T>& v) const override
    {
      return v.visit(*this);
    }

    token keyword;
  };

  template<typename T>
  struct expr_unary : public expression<T> 
  {
    using expr_t = expression<T>;
    expr_unary(token op, expr_t* right) : op(op), right(right) {}
    ~expr_unary() 
    {
      if (right) {delete right;}
    }

    T accept(const expr_visitor<T>& v) const override
    {
      return v.visit(*this);
    }

    token op;
    expr_t* right; 
  };

  template<typename T>
  struct expr_variable : public expression<T> 
  {
    using expr_t = expression<T>;
    expr_variable(token name) : name(name) {}

    T accept(const expr_visitor<T>& v) const override
    {
      return v.visit(*this);
    }

    token name;
  };

} // namespace cwt
