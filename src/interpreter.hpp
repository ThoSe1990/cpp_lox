#pragma once 

#include <vector>


#include "expr.hpp"
#include "stmt.hpp"
#include "lox_obj.hpp"

#include "environment.hpp"


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
      environment* get_env_ptr();
      void interpret(const std::vector<stmt_t>& statements);

      void execute(const stmt_t& statement);
      void execute(const std::vector<stmt_t>& statements);
      void execute_block(const std::vector<stmt_t>& statements, std::unique_ptr<environment> new_env);

      void visit(const stmt_block<lox_obj>& s) override ;
      void visit(const stmt_expression<lox_obj>& s) override ;
      void visit(const stmt_if<lox_obj>& s) override;
      void visit(const stmt_print<lox_obj>& s) override ;
      void visit(const stmt_var<lox_obj>& s) override;
      void visit(const stmt_while<lox_obj>& s) override;
      void visit(const stmt_function<lox_obj>& s) override;
      void visit(const stmt_return<lox_obj>& s) override;

      lox_obj visit(const expr_assign<lox_obj>& e) override;
      lox_obj visit(const expr_literal<lox_obj>& e) override;
      lox_obj visit(const expr_logical<lox_obj>& e) override ;
      lox_obj visit(const expr_grouping<lox_obj>& e) override;
      lox_obj visit(const expr_unary<lox_obj>& e) override;
      lox_obj visit(const expr_variable<lox_obj>& e) override;
      lox_obj visit(const expr_binary<lox_obj>& e) override;
      lox_obj visit(const expr_call<lox_obj>& e) override;

    private:
      lox_obj evaluate(const expr_t& e)  ;
      bool is_truthy(const lox_obj& obj)  ;
      bool is_equal(const lox_obj& left, const lox_obj& right) const ;
      
      void check_number_operand(const token& op, const lox_obj& operand) const ;
      void check_number_operand(const token& op, const lox_obj& left, const lox_obj& right) const ;
    private:
      std::unique_ptr<environment> m_env = std::make_unique<environment>();
  };
} // namespace cwt
