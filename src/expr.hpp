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
  struct visitor 
  {
    virtual ~visitor() = default;
    virtual T visit(const expr_assign<T>& e) const { return "not implemented"; }
    virtual T visit(const expr_binary<T>& e) const { return "not implemented"; }
    virtual T visit(const expr_call<T>& e) const { return "not implemented"; }
    virtual T visit(const expr_get<T>& e) const { return "not implemented"; }
    virtual T visit(const expr_grouping<T>& e) const { return "not implemented"; }
    virtual T visit(const expr_literal<T>& e) const { return "not implemented"; }
    virtual T visit(const expr_logical<T>& e) const { return "not implemented"; }
    virtual T visit(const expr_set<T>& e) const { return "not implemented"; }
    virtual T visit(const expr_super<T>& e) const { return "not implemented"; }
    virtual T visit(const expr_this<T>& e) const { return "not implemented"; }
    virtual T visit(const expr_unary<T>& e) const { return "not implemented"; }
    virtual T visit(const expr_variable<T>& e) const { return "not implemented"; }
  };

  template<typename T>
  struct expression
  {
    virtual ~expression() = default;
    virtual T accept(const visitor<T>& v) const = 0 ;
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
    std::string accept(const visitor<T>& v) const override
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
    std::string accept(const visitor<T>& v) const override
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
    expr_call(expr_t* callee, token paren, std::vector<expr_t*> args) : callee(callee), paren(paren), args(args) {}
  	~expr_call()
    {
      if (callee) {delete callee;}
      for (auto e : args) {if (e) delete e;}
    }
    std::string accept(const visitor<T>& v) const override
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
    std::string accept(const visitor<T>& v) const override
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
    std::string accept(const visitor<T>& v) const override
    {
      return v.visit(*this);
    }

    expr_t* expr;
  };

  template<typename T>
  struct expr_literal : public expression<T> 
  {
    using expr_t = expression<T>;
    expr_literal(const std::string& value) : value(value) {}
    ~expr_literal(){}

    std::string accept(const visitor<T>& v) const override
    {
      return v.visit(*this);
    }

    std::string value;
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
    std::string accept(const visitor<T>& v) const override
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
    std::string accept(const visitor<T>& v) const override
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
    std::string accept(const visitor<T>& v) const override
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

    std::string accept(const visitor<T>& v) const override
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

    std::string accept(const visitor<T>& v) const override
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

    std::string accept(const visitor<T>& v) const override
    {
      return v.visit(*this);
    }

    token name;
  };



  class printer : public visitor<std::string>
  {
    public:
      ~printer() = default; 
      void print(expression<std::string>& expr) const
      {
        std:: cout<< expr.accept(*this) << std::endl;
      }

      std::string visit(const expr_binary<std::string>& e) const override
      {
        return parenthesize(e.op.m_lexeme, *e.left, *e.right);
      }

      std::string visit(const expr_grouping<std::string>& e) const override
      {
        return parenthesize("group", *e.expr);
      }

      std::string visit(const expr_literal<std::string>& e) const override
      {
        return e.value.empty() ? "nil" : e.value;
      }

      std::string visit(const expr_unary<std::string>& e) const override
      {
        return parenthesize(e.op.m_lexeme, *e.right);
      }

    private:

      std::string parenthesize(const expression<std::string>& expr) const 
      {
        std::string s{' '};
        s.append(expr.accept(*this));
        return s;
      }

      template<typename... Exprs>
      std::string parenthesize(const std::string& name, const expression<std::string>& expr, const Exprs&... exprs) const 
      {
        std::string s{""};
        s.push_back('(');
        s.append(name);
        s.append(parenthesize(expr));
        s.append(parenthesize(exprs...));
        s.push_back(')');
        return s;
      }
      std::string parenthesize(const std::string& name, const expression<std::string>& expr) const 
      {   
        std::string s{""};
        s.push_back('(');
        s.append(name);
        s.append(parenthesize(expr));
        s.push_back(')');
        return s;
      }

  };

} // namespace cwt
