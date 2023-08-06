#pragma once 

namespace cwt
{ 
  template<typename T> 
  class parser
  {
    using value_t = T;
    using expr_t = expression<value_t>;
    
    public: 
      parser(const std::vector<token>& tokens) : m_tokens(tokens) {}

      expr_t* parse() 
      {
        try
        {
          return expression();
        }
        catch(const std::exception& e)
        {
          std::cout << e.what() << '\n';
          return nullptr;
        }
      }

    private:
      expr_t* expression() 
      {
        return equality();
      }
      expr_t* equality() 
      {
        expr_t* expr = comparison();
        while (match(token_type::BANG_EQUAL, token_type::EQUAL_EQUAL))
        {
          token op = previous();
          expr_t* right = comparison();
          expr = new expr_binary<value_t>(expr, op, right);
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

      template<typename... T>
      bool match(token_type head, T... ts)
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

      expr_t* comparison()
      {
        expr_t* expr = term();
        while(match(token_type::GREATER, token_type::GREATER_EQUAL, token_type::LESS, token_type::LESS_EQUAL))
        {
          token op = previous();
          expr_t* right = term();
          expr = new cwt::expr_binary<value_t>(expr, op, right);
        }
        return expr;
      }

      expr_t* term()
      {
        expr_t* expr = factor();
        while(match(token_type::MINUS, token_type::PLUS))
        {
          token op = previous();
          expr_t* right = factor();
          expr = new expr_binary<value_t>(expr, op, right);
        }
        return expr; 
      }

      expr_t* factor()
      {
        expr_t* expr = unary();
        while (match(token_type::SLASH, token_type::STAR))
        {
          token op = previous();
          expr_t* right = unary();
          expr = new expr_binary<value_t>(expr, op, right);
        }
        return expr; 
      }

      expr_t* unary()
      {
        if (match(token_type::BANG, token_type::MINUS))
        {
          token op = previous();
          expr_t* right = unary();
          return new expr_unary<value_t>(op, right);
        }
        return primary();
      }

      expr_t* primary()
      {
        if (match(token_type::FALSE)) return new expr_literal<value_t>(false);
        if (match(token_type::TRUE)) return new expr_literal<value_t>(true);
        if (match(token_type::NIL)) return new expr_literal<value_t>();
        if (match(token_type::NUMBER))
        {
          return new expr_literal<value_t>(std::stod(previous().literal));
        }
        if (match(token_type::STRING))
        {
          return new expr_literal<value_t>(previous().literal);
        }
        if (match(token_type::LEFT_PAREN))
        {
          expr_t* expr = expression();
          consume(token_type::RIGHT_PAREN, "Expect: \')\' after expression.");
          return new expr_grouping<value_t>(expr);
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
