#pragma once 

namespace cwt
{
  class printer : public expr_visitor<std::string>
  {
    public:
      ~printer() = default; 
      void print(expression<std::string>& expr) const
      {
        std:: cout<< expr.accept(*this) << std::endl;
      }

      std::string visit(const expr_binary<std::string>& e) const override
      {
        return parenthesize(e.op.lexeme, *e.left, *e.right);
      }

      std::string visit(const expr_grouping<std::string>& e) const override
      {
        return parenthesize("group", *e.expr);
      }

      std::string visit(const expr_literal<std::string>& e) const override
      {
        return e.value.to_string();
      }

      std::string visit(const expr_unary<std::string>& e) const override
      {
        return parenthesize(e.op.lexeme, *e.right);
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
