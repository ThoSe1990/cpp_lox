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

  struct token 
  {  
      token(const token_type type, const std::string& lexeme, const std::size_t line) 
      : type(type), lexeme(lexeme), line(line) {}

      token(const token_type type, const std::string& lexeme, const std::size_t line, const std::string& literal) 
      : type(type), lexeme(lexeme), line(line), literal(literal) {}

      std::string to_string() const noexcept
      {
        std::string s{std::to_string(static_cast<std::size_t>(type))};
        s.append(" ");
        s.append(lexeme);
        s.append(" ");
        s.append(literal);
        return s;
      }
 
      token_type type; 
      std::string lexeme; 
      std::size_t line;
      std::string literal;
  };

} // namespace cwt
