#pragma once 

namespace cwt
{ 
  template<typename T> 
  class parser
  {
    using value_t = T;
    using expr_t = lox_expression<value_t>;
    using stmt_t = lox_statement<value_t>;
    
    public: 
      parser(const std::vector<token>& tokens) : m_tokens(tokens) {}

      std::vector<std::unique_ptr<stmt_t>> parse()
      {
        std::vector<std::unique_ptr<stmt_t>> statements;
        while(!is_at_end())
        {
          for (auto& d : declaration()) 
          {
            statements.push_back(std::move(d));
          }
        }
        return statements;
      }

    private:
      
      std::vector<std::unique_ptr<stmt_t>> declaration()
      {
        try
        {
          if (match(token_type::VAR)) 
          {
            std::vector<std::unique_ptr<stmt_t>> v;
            v.push_back(std::move(var_declaration()));
            return v;
          }
          else 
          {
            return statement();
          }
        }
        catch(const std::exception& e)
        {
          synchronize();
          std::cerr << e.what() << '\n';
          return {};
        }
      }
      
      std::unique_ptr<stmt_t> var_declaration()
      {
        token name = consume(token_type::IDENTIFIER, "Expected variable name.");
        std::unique_ptr<expr_t> initializer = nullptr;
        if (match(token_type::EQUAL))
        {
          initializer = expression();
        }
        consume(token_type::SEMICOLON, "Expected \';\' after variable declaration");
        return std::make_unique<stmt_var<value_t>>(name, std::move(initializer));
      }
      std::vector<std::unique_ptr<stmt_t>> statement()
      {
        if (match(token_type::PRINT)) 
        {   
          std::vector<std::unique_ptr<stmt_t>> v;
          v.push_back(std::move(print_statement())); 
          return v; 
        }
        else if (match(token_type::IF)) 
        { 
          std::vector<std::unique_ptr<stmt_t>> v;
          v.push_back(std::move(if_statement())); 
          return v;  
        }
        else if (match(token_type::LEFT_BRACE)) { return block(); }
        else 
        { 
          std::vector<std::unique_ptr<stmt_t>> v;
          v.push_back(std::move(expression_statement())); 
          return v; 
        }
      }

      std::unique_ptr<stmt_t> if_statement()
      {
        consume(token_type::LEFT_PAREN, "Expect \'(\' after \'if\'.");
        std::unique_ptr<expr_t> condition = expression();
        consume(token_type::RIGHT_PAREN, "Expect \')\' after if condition.");
        std::vector<std::unique_ptr<stmt_t>> then_branch = statement();
        std::vector<std::unique_ptr<stmt_t>> else_branch{};
        if (match(token_type::ELSE))
        {
          else_branch = statement();
        }
        return std::make_unique<stmt_if<value_t>>(std::move(condition), std::move(then_branch), std::move(else_branch));
      }

      std::unique_ptr<stmt_t> print_statement()
      {
        std::unique_ptr<expr_t> value = expression();
        consume(token_type::SEMICOLON, "Expect \';\' after value.");
  
        return std::make_unique<stmt_print<value_t>>(std::move(value));
      }
      
      std::unique_ptr<stmt_t> expression_statement()
      {
        std::unique_ptr<expr_t> expr = expression();
        consume(token_type::SEMICOLON, "Expect \';\' after expression.");
        return std::make_unique<stmt_expression<value_t>>(std::move(expr));
      }

      std::vector<std::unique_ptr<stmt_t>> block()
      {
        std::vector<std::unique_ptr<stmt_t>> statements; 

        while (!check(token_type::RIGHT_BRACE) && !is_at_end())
        {
          for (auto& d : declaration()) 
          {
            statements.push_back(std::move(d));
          }
        }
        consume(token_type::RIGHT_BRACE, "Expected \'}\' after block");
        return statements;
      }

      std::unique_ptr<expr_t> expression() 
      {
        return assignment();
      }

      std::unique_ptr<expr_t> assignment()
      {
        std::unique_ptr<expr_t> expr = or_operator();
        if(match(token_type::EQUAL))
        {
          token equals = previous();
          std::unique_ptr<expr_t> value = assignment();
          if(expr->type() == expr_type::_variable)
          {
            
            token name = static_cast<expr_variable<value_t>*>(expr.get())->name;
            return std::make_unique<expr_assign<lox_obj>>(name, std::move(value));
          }
          else
          {
            error(equals, "Invalid assignment target.");
          }
        }
        return expr;
      }

      
      std::unique_ptr<expr_t> or_operator() 
      {
        std::unique_ptr<expr_t> expr = and_operator();
        while(match(token_type::OR))
        {
          token op = previous();
          std::unique_ptr<expr_t> right = and_operator();
          expr = std::make_unique<expr_logical<value_t>>(std::move(expr), op, std::move(right));
        }
        return std::move(expr);
      }
      std::unique_ptr<expr_t> and_operator() 
      {
        std::unique_ptr<expr_t> expr = equality();
        while (match(token_type::AND))
        {
          token op = previous();
          std::unique_ptr<expr_t> right = equality();
          expr = std::make_unique<expr_logical<value_t>>(std::move(expr), op, std::move(right));
        }
        return std::move(expr);
      }
      
      std::unique_ptr<expr_t> equality() 
      {
        std::unique_ptr<expr_t> expr = comparison();
        while (match(token_type::BANG_EQUAL, token_type::EQUAL_EQUAL))
        {
          token op = previous();
          std::unique_ptr<expr_t> right = comparison();
          expr = std::make_unique<expr_binary<value_t>>(std::move(expr), op, std::move(right));
        }
        return expr; 
      }

      bool match(token_type t)
      {
        if (check(t)) {
          advance();
          return true;
        }
        return false;
      }

      template<typename... Tokens>
      bool match(token_type head, Tokens... ts)
      {
        return match(head) ? true : match(ts...);
      }

      bool check(token_type t)
      {
        if (is_at_end())
        {
          return false;
        }
        else 
        {
          return peek().type == t;
        }
      }

      token advance()
      {
        if (!is_at_end()) ++m_current;
        return previous();
      }

      bool is_at_end() 
      {
        return peek().type == token_type::END_OF_FILE;
      }

      token peek()
      {
        return m_tokens[m_current];
      }

      token previous()
      {
        return m_tokens[m_current-1];
      }

      std::unique_ptr<expr_t> comparison()
      {
        std::unique_ptr<expr_t> expr = term();
        while(match(token_type::GREATER, token_type::GREATER_EQUAL, token_type::LESS, token_type::LESS_EQUAL))
        {
          token op = previous();
          std::unique_ptr<expr_t> right = term();
          expr = std::make_unique<cwt::expr_binary<value_t>>(std::move(expr), op, std::move(right));
        }
        return expr;
      }

      std::unique_ptr<expr_t> term()
      {
        std::unique_ptr<expr_t> expr = factor();
        while(match(token_type::MINUS, token_type::PLUS))
        {
          token op = previous();
          std::unique_ptr<expr_t> right = factor();
          expr = std::make_unique<expr_binary<value_t>>(std::move(expr), op, std::move(right));
        }
        return expr; 
      }

      std::unique_ptr<expr_t> factor()
      {
        std::unique_ptr<expr_t> expr = unary();
        while (match(token_type::SLASH, token_type::STAR))
        {
          token op = previous();
          std::unique_ptr<expr_t> right = unary();
          expr = std::make_unique<expr_binary<value_t>>(std::move(expr), op, std::move(right));
        }
        return expr; 
      }

      std::unique_ptr<expr_t> unary()
      {
        if (match(token_type::BANG, token_type::MINUS))
        {
          token op = previous();
          std::unique_ptr<expr_t> right = unary();
          return std::make_unique<expr_unary<value_t>>(op, std::move(right));
        }
        return primary();
      }

      std::unique_ptr<expr_t> primary()
      {
        if (match(token_type::FALSE)) return std::make_unique<expr_literal<value_t>>(false);
        if (match(token_type::TRUE)) return std::make_unique<expr_literal<value_t>>(true);
        if (match(token_type::NIL)) return std::make_unique<expr_literal<value_t>>();
        if (match(token_type::NUMBER))
        {
          return std::make_unique<expr_literal<value_t>>(std::stod(previous().literal));
        }
        if (match(token_type::STRING))
        {
          return std::make_unique<expr_literal<value_t>>(previous().literal);
        }
        if (match(token_type::IDENTIFIER))
        {
          return std::make_unique<expr_variable<value_t>>(previous());
        }
        if (match(token_type::LEFT_PAREN))
        {
          std::unique_ptr<expr_t> expr = expression();
          consume(token_type::RIGHT_PAREN, "Expect: \')\' after expression.");
          return std::make_unique<expr_grouping<value_t>>(std::move(expr));
        }
        throw std::runtime_error(error(peek(), "Expected expression."));
      }

      token consume(token_type type, const std::string& msg) 
      {
        if(check(type)) {
          return advance();
        }
        throw std::runtime_error(error(peek(), msg));
      }

      std::string error(token t, const std::string& msg) 
      {
        if (t.type == token_type::END_OF_FILE) 
        {
          report(t.line, " at end ", msg);
        }
        else 
        {
          std::string s{"at \' "};
          s.append(t.lexeme);
          s.append("\'");
          report(t.line, s, msg);
        }
        
        return msg; 
      }

      void synchronize() 
      {
        advance();
        while (!is_at_end())
        {
          if (previous().type == token_type::SEMICOLON) return; 
          switch (peek().type)
          {
          case token_type::CLASS :
          case token_type::FOR :
          case token_type::FUN :
          case token_type::IF :
          case token_type::PRINT :
          case token_type::RETURN :
          case token_type::VAR :
          case token_type::WHILE :
            return;
          }
          advance();
        }
      }

    private:
      std::size_t m_current{0};
      std::vector<token> m_tokens;
  };
} // namespace cwt
