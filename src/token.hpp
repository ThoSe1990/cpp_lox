#pragma once 

namespace cwt
{
  enum class token_type {
    // single character tokens
    LEFT_PAREN = 0, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
    COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR, POUND,

    // one or two character tokens
    BANG, BANG_EQUAL, EQUAL, EQUAL_EQUAL,
    GREATER, GREATER_EQUAL, LESS, LESS_EQUAL,

    // literals 
    IDENTIFIER, STRING, NUMBER, 

    // keywords 
    AND, CLASS, ELSE, FALSE, FUN, FOR, IF, NIL, OR, 
    PRINT, RETURN, SUPER, THIS, TRUE, VAR, WHILE,

    END_OF_FILE
  };

  class token 
  { 
    public: 
      token(const token_type type, const std::string& lexeme, const std::size_t line) 
      : m_type(type), m_lexeme(lexeme), m_line(line) {}

      token(const token_type type, const std::string& lexeme, const std::size_t line, const std::string& literal) 
      : m_type(type), m_lexeme(lexeme), m_line(line), m_literal(literal) {}

      std::string to_string() const noexcept
      {
        std::string s{std::to_string(static_cast<std::size_t>(m_type))};
        s.append(" ");
        s.append(m_lexeme);
        s.append(" ");
        s.append(m_literal);
        return s;
      }
    private: 
      token_type m_type; 
      std::string m_lexeme; 
      std::size_t m_line;
      std::string m_literal;
  };

} // namespace cwt
