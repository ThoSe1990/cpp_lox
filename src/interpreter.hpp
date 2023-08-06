#pragma once 

namespace cwt
{
  class interpreter : public expr_visitor<lox_obj>, public stmt_visitor<lox_obj>
  {
    using expr_t = expression<lox_obj>;
    using stmt_t = statement<lox_obj>;
    public:
      void interpret(const std::vector<stmt_t*> statemets) 
      {
        try
        {
          for (stmt_t* stmt : statemets)
          {
            execute(stmt);
          }
        }
        catch(const std::exception& e)
        {
          std::cerr << e.what() << '\n';
        }
      }

      void visit(const stmt_expression<lox_obj>& s) override 
      {
        evaluate(s.expression);
      }
      void visit(const stmt_print<lox_obj>& s) override 
      {
        lox_obj value = evaluate(s.expression);
        std::cout << value.to_string() << std::endl;
      }
      void visit(const stmt_var<lox_obj>& s) override
      {
        lox_obj value;
        if (s.initializer)
        {
          value = evaluate(s.initializer);
        }
        m_env.define(s.name.lexeme, value);
      }

      lox_obj visit(const expr_literal<lox_obj>& e) override
      {
        return create_another(e.value);
      }

      lox_obj visit(const expr_grouping<lox_obj>& e) override
      {
        return evaluate(e.expr);
      }

      lox_obj visit(const expr_unary<lox_obj>& e) override
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

      lox_obj visit(const expr_variable<lox_obj>& e) override
      {
        return create_another(m_env.get(e.name));
      }

      lox_obj visit(const expr_binary<lox_obj>& e) override
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
            if (left.type() == lox_obj::value_type::number && right.type() == lox_obj::value_type::number)
            {
              return left.number() + right.number();
            }
            else if (left.type() == lox_obj::value_type::string && right.type() == lox_obj::value_type::string)
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
    private:
      void execute(stmt_t* stmt)
      {
        stmt->accept(*this);
      }
      lox_obj evaluate(expr_t* e)  
      {
        return e->accept(*this);
      }
      bool is_truthy(const lox_obj& obj)  
      {
        if (obj.nil())
        {
          return false;
        }
        else if (obj.type() == lox_obj::value_type::boolean) 
        {
          return obj.boolean();
        }
        else 
        {
          return true;
        }
      }
      bool is_equal(const lox_obj& left, const lox_obj& right) const 
      {
        auto both_type = [&left, &right](lox_obj::value_type t) { return left.type() == t && right.type() == t; };

        if (left.nil() && right.nil()) 
        {
          return true;
        }
        else if (both_type(lox_obj::value_type::number))
        {
          return left.number() == right.number();
        }
        else if (both_type(lox_obj::value_type::boolean))
        {
          return left.boolean() == right.boolean();
        }
        else if (both_type(lox_obj::value_type::string))
        {
          return left.string() == right.string();
        }
        else 
        {
          return false; 
        }
      }

      void check_number_operand(const token& op, const lox_obj& operand) const 
      {
        if (operand.type() == lox_obj::value_type::number) 
        {
          return;
        }
        else 
        {
          runtime_error(op, " Operand must be a number.");
        }
      }
      void check_number_operand(const token& op, const lox_obj& left, const lox_obj& right) const 
      {
        if (left.type() == lox_obj::value_type::number && right.type() == lox_obj::value_type::number) 
        {
          return;
        }
        else 
        {
          runtime_error(op, " Operands must be a number.");
        }
      }
    private:
      environment m_env;
  };
} // namespace cwt
