namespace cwt
{   
  
  struct expr_assign;
  struct expr_binary;
  struct expr_call;
  struct expr_get;
  struct expr_grouping;
  struct expr_literal;
  struct expr_logical;
  struct expr_set;
  struct expr_super;
  struct expr_this;
  struct expr_unary;
  struct expr_variable;

  struct visitor 
  {
    virtual ~visitor() = default;
    virtual std::string visit(const expr_assign& e) const { return "not implemented"; }
    virtual std::string visit(const expr_binary& e) const { return "not implemented"; }
    virtual std::string visit(const expr_call& e) const { return "not implemented"; }
    virtual std::string visit(const expr_get& e) const { return "not implemented"; }
    virtual std::string visit(const expr_grouping& e) const { return "not implemented"; }
    virtual std::string visit(const expr_literal& e) const { return "not implemented"; }
    virtual std::string visit(const expr_logical& e) const { return "not implemented"; }
    virtual std::string visit(const expr_set& e) const { return "not implemented"; }
    virtual std::string visit(const expr_super& e) const { return "not implemented"; }
    virtual std::string visit(const expr_this& e) const { return "not implemented"; }
    virtual std::string visit(const expr_unary& e) const { return "not implemented"; }
    virtual std::string visit(const expr_variable& e) const { return "not implemented"; }
  };

  struct expression
  {
    virtual ~expression() = default;
    virtual std::string accept(const visitor& v) const = 0;
  };

  struct expr_assign : public expression 
  {
    expr_assign(token op, expression* value) : op(op), value(value) {}
    ~expr_assign()  
    {
      if (value) {delete value;}
    }
    std::string accept(const visitor& v) const override
    {
      return v.visit(*this);
    } 

    token op;
    expression* value;
  };

  struct expr_binary : public expression 
  {
    expr_binary(expression* left, token op, expression* right) : left(left), op(op), right(right) {}
    ~expr_binary() 
    {
      if (left) {delete left;}
      if (right) {delete right;}
    }
    std::string accept(const visitor& v) const override
    {
      return v.visit(*this);
    }
    expression* left;
    token op;
    expression* right;
  };

  struct expr_call : public expression 
  {
    expr_call(expression* callee, token paren, std::vector<expression*> args) : callee(callee), paren(paren), args(args) {}
  	~expr_call()
    {
      if (callee) {delete callee;}
      for (auto e : args) {if (e) delete e;}
    }
    std::string accept(const visitor& v) const override
    {
      return v.visit(*this);
    }

    expression* callee;
    token paren;
    std::vector<expression*> args; 

  };

  struct expr_get : public expression 
  {
    expr_get(expression* obj, token name) : obj(obj), name(name) {}
    ~expr_get()
    {
      if (obj) { delete obj; }
    }
    std::string accept(const visitor& v) const override
    {
      return v.visit(*this);
    }

    expression* obj;
    token name; 
  };

  struct expr_grouping : public expression 
  {
    expr_grouping(expression* expr) : expr(expr) {}
    ~expr_grouping()  
    {
      if (expr) {delete expr;}
    }
    std::string accept(const visitor& v) const override
    {
      return v.visit(*this);
    }

    expression* expr;
  };

  struct expr_literal : public expression 
  {
    expr_literal(const std::string& value) : value(value) {}
    ~expr_literal(){}

    std::string accept(const visitor& v) const override
    {
      return v.visit(*this);
    }

    std::string value;
  };

  struct expr_logical : public expression 
  {
    expr_logical(expression* left, token op, expression* right) : left(left), op(op), right(right) {}
    ~expr_logical() 
    {
      if (left) {delete left;}
      if (right) {delete right;}
    }
    std::string accept(const visitor& v) const override
    {
      return v.visit(*this);
    }

    expression* left;
    token op;
    expression* right;
  };

  struct expr_set : public expression 
  {
    expr_set(expression* obj, token name, expression* value) : obj(obj), name(name), value(value) {}
    ~expr_set() 
    {
      if (obj) { delete obj; }
      if (value) { delete value; }
    }
    std::string accept(const visitor& v) const override
    {
      return v.visit(*this);
    }

    expression* obj;
    token name; 
    expression* value; 
  };

  struct expr_super : public expression 
  {
    expr_super(token keyword, token method) : keyword(keyword), method(method) {}
    ~expr_super() = default;
    std::string accept(const visitor& v) const override
    {
      return v.visit(*this);
    }

    token keyword;
    token method;
  };

  struct expr_this : public expression 
  {
    expr_this(token keyword) : keyword(keyword) {}
    ~expr_this() = default;

    std::string accept(const visitor& v) const override
    {
      return v.visit(*this);
    }

    token keyword;
  };

  struct expr_unary : public expression 
  {
    expr_unary(token op, expression* right) : op(op), right(right) {}
    ~expr_unary() 
    {
      if (right) {delete right;}
    }

    std::string accept(const visitor& v) const override
    {
      return v.visit(*this);
    }

    token op;
    expression* right; 
  };

  struct expr_variable : public expression 
  {
    expr_variable(token name) : name(name) {}

    std::string accept(const visitor& v) const override
    {
      return v.visit(*this);
    }

    token name;
  };




  class printer : public visitor
  {
    public:
      ~printer() = default; 
      void print(expression& expr) const
      {
        std:: cout<< expr.accept(*this) << std::endl;
      }

      std::string visit(const expr_binary& e) const override
      {
        return parenthesize(e.op.m_lexeme, *e.left, *e.right);
      }

      std::string visit(const expr_grouping& e) const override
      {
        return parenthesize("group", *e.expr);
      }

      std::string visit(const expr_literal& e) const override
      {
        return e.value.empty() ? "nil" : e.value;
      }

      std::string visit(const expr_unary& e) const override
      {
        return parenthesize(e.op.m_lexeme, *e.right);
      }

    private:

      std::string parenthesize(const expression& expr) const 
      {
        std::string s{' '};
        s.append(expr.accept(*this));
        return s;
      }

      template<typename... Exprs>
      std::string parenthesize(const std::string& name, const expression& expr, const Exprs&... exprs) const 
      {
        std::string s{""};
        s.push_back('(');
        s.append(name);
        s.append(parenthesize(expr));
        s.append(parenthesize(exprs...));
        s.push_back(')');
        return s;
      }
      std::string parenthesize(const std::string& name, const expression& expr) const 
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
