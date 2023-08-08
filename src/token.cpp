#include "token.hpp"

namespace cwt
{
  
token::token(const token_type type, const std::string& lexeme, const std::size_t line) 
: type(type), lexeme(lexeme), line(line) {}

token::token(const token_type type, const std::string& lexeme, const std::size_t line, const std::string& literal) 
: type(type), lexeme(lexeme), line(line), literal(literal) {}

std::string token::to_string() const noexcept
{
  std::string s{std::to_string(static_cast<std::size_t>(type))};
  s.append(" ");
  s.append(lexeme);
  s.append(" ");
  s.append(literal);
  return s;
}

} // namespace cwt
