#pragma once 

namespace cwt
{
  template<typename Func> 
  class finally 
  {
    public:
      finally(Func&& func) : m_func(func) {}
      ~finally() { m_func(); }
    private:
      Func m_func;
  };


  class interpreter : public expr_visitor<lox_obj>, public stmt_visitor<lox_obj>
  {
    using expr_t = std::unique_ptr<lox_expression<lox_obj>>;
    using stmt_t = std::unique_ptr<lox_statement<lox_obj>>;
    
    public:
      void interpret(const std::vector<stmt_t>& statements) 
      {
        try
        {
          execute(statements);
        }
        catch(const std::exception& e)
        {
          std::cerr << e.what() << '\n';
        }
      }

      void visit(const stmt_block<lox_obj>& s) override 
      {
        execute_block(s.statements);
      }
      void visit(const stmt_expression<lox_obj>& s) override 
      {
        evaluate(s.expression);
      }
      void visit(const stmt_if<lox_obj>& s) override
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
        m_env->define(s.name.lexeme, value);
      }
      lox_obj visit(const expr_assign<lox_obj>& e) override
      {
        lox_obj value = evaluate(e.value);
        m_env->assign(e.name, value);
        return create_another(value);
      }

      lox_obj visit(const expr_literal<lox_obj>& e) override
      {
        return create_another(e.value);
      }

      lox_obj visit(const expr_logical<lox_obj>& e) override 
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
        return create_another(m_env->get(e.name));
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
    private:
      void execute(const stmt_t& statement)
      {
        statement->accept(*this);
      }
      void execute(const std::vector<stmt_t>& statements)
      {
        for (const auto& s : statements)
        {
          s->accept(*this);
        }
      }

      void execute_block(const std::vector<stmt_t>& statements)
      {
        std::unique_ptr<environment> prev = std::move(m_env);
        try
        {
          finally on_exit([this, &prev]()
          { 
            m_env = std::move(prev); 
          });

          m_env = std::make_unique<environment>();
          m_env->set_enclosing(prev.get());
          execute(statements);
        }
        catch(const std::exception& e)
        {
          std::cerr << e.what() << '\n';
        }
      }
      

      lox_obj evaluate(const expr_t& e)  
      {
        return e->accept(*this);
      }
      bool is_truthy(const lox_obj& obj)  
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
      bool is_equal(const lox_obj& left, const lox_obj& right) const 
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

      void check_number_operand(const token& op, const lox_obj& operand) const 
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
      void check_number_operand(const token& op, const lox_obj& left, const lox_obj& right) const 
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
    private:
      std::unique_ptr<environment> m_env = std::make_unique<environment>();
  };
} // namespace cwt
