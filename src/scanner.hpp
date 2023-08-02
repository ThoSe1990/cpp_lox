#pragma once 

#include <unordered_map>

namespace cwt
{

  class scanner 
  {
    public:
      scanner(const std::string& src) : m_src(src) {}

      std::vector<token> scan_tokens()
      {
        while(!is_at_end())
        {
          m_start = m_current;
          scan_token();
        }

        m_tokens.push_back(token{token_type::END_OF_FILE, "", m_line});
        return m_tokens;    
      }

    private:
      void scan_token()
      {
        char c = advance();
        switch (c)
        {
          case '(': add_token(token_type::LEFT_PAREN);
          break; case ')': add_token(token_type::RIGHT_PAREN);
          break; case '{': add_token(token_type::LEFT_BRACE);
          break; case '}': add_token(token_type::RIGHT_BRACE);
          break; case ',': add_token(token_type::COMMA);
          break; case '.': add_token(token_type::DOT);
          break; case '-': add_token(token_type::MINUS);
          break; case '+': add_token(token_type::PLUS);
          break; case '/': add_token(token_type::SLASH);
          break; case ';': add_token(token_type::SEMICOLON);
          break; case '*': add_token(token_type::STAR);
          break; case '!': add_token( match('=') ? token_type::BANG_EQUAL : token_type::BANG );
          break; case '=': add_token( match('=') ? token_type::EQUAL_EQUAL : token_type::EQUAL );
          break; case '<': add_token( match('=') ? token_type::LESS_EQUAL : token_type::LESS );
          break; case '>': add_token( match('=') ? token_type::GREATER_EQUAL : token_type::GREATER );
          break; case '#': { while(peek() != '\n' && !is_at_end()) advance(); }
          break; case ' ': case '\r': case '\t' :
          break; case '\n': ++m_line;
          break; case '\"': do_string();
          break; case 'o': if (match('r')) { add_token(token_type::OR); }
          break; default: 
          {
            if (is_digit(c)) 
            {
              do_number();
            }
            else if (is_alpha(c)) 
            {
              identifier();
            }
            else 
            {
              cwt::error(m_line, "unexpected character");
              return;
            }
          }
        }
      }
      
      void identifier()
      {
        while (is_alpha_numeric(peek())) advance();
        // substr works different in cpp as in java
        std::string txt = m_src.substr(m_start, m_current-m_start);
        if (m_keywords.count(txt)) {
          add_token(m_keywords.at(txt));
        } else {
          add_token(token_type::IDENTIFIER);
        }
      }

      bool is_alpha_numeric(const char c)
      {
        return is_alpha(c) || is_digit(c);
      }

      bool is_alpha(const char c) 
      {
        return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
      }

      char peek_next() 
      {
        if (m_current+1 >= m_src.length()) return '\0';
        return m_src[m_current+1];
      }

      void do_number()
      {
        while (is_digit(peek())) advance();

        if (peek() == '.' && is_digit(peek_next())) {
          // consume '.'
          advance();
          while (is_digit(peek())) advance();
        }
                                                // substr works different in cpp as in java
        add_token(token_type::NUMBER, m_src.substr(m_start, m_current-m_start));
      }

      bool is_digit(const char c) 
      {
        return c >= '0' && c <= '9';
      }

      void do_string()
      {
        while (peek() != '\"' && !is_at_end()) 
        {
          if (peek() == '\n') ++m_line;
          advance();
        }
        if (is_at_end()) 
        {
          cwt::error(m_line, "unterminated string");
          return;
        }
        advance();
        // substr works different in cpp as in java
        std::string value = m_src.substr(m_start+1, (m_current-1)-(m_start+1));
        add_token(token_type::STRING, value);
      }

      char peek() 
      {
        if (is_at_end()) return '\0';
        return m_src[m_current];
      }

      bool is_at_end()
      {
        return m_current >= m_src.length();
      }

      void add_token(token_type type) 
      {
        add_token(type, "");
      }
      void add_token(token_type type, const std::string& literal) 
      {
        // substr works different in cpp as in java
        const std::string txt = m_src.substr(m_start, m_current-m_start);
        m_tokens.push_back(token{type, txt, m_line, literal});
      }
      char advance() 
      { 
        return m_src[m_current++]; 
      }

      bool match(const char expected) 
      {
        if (is_at_end()) return false;
        if (m_src[m_current] != expected) return false;
        ++m_current;
        return true;
      }

    private: 
      std::string m_src;
      std::vector<token> m_tokens{};
      std::size_t m_start{0};
      std::size_t m_current{0};
      std::size_t m_line{1};  
      const std::unordered_map<std::string, token_type> m_keywords {
        {"and", token_type::AND},
        {"class", token_type::CLASS},
        {"else", token_type::ELSE},
        {"false", token_type::FALSE},
        {"for", token_type::FOR},
        {"fun", token_type::FUN},
        {"if", token_type::IF},
        {"nil", token_type::NIL},
        {"or", token_type::OR},
        {"print", token_type::PRINT},
        {"return", token_type::RETURN},
        {"super", token_type::SUPER},
        {"this", token_type::THIS},
        {"true", token_type::TRUE},
        {"var", token_type::VAR},
        {"while", token_type::WHILE},
      };
    };
} // namespace cwt
