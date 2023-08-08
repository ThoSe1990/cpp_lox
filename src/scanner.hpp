#pragma once 

#include <unordered_map>
#include <string>
#include <vector>

#include "error.hpp"
#include "token.hpp"
namespace cwt
{

  class scanner 
  {
    public:
      scanner(const std::string& src);

      std::vector<token> scan_tokens();

    private:
      void scan_token();
      
      void identifier();

      bool is_alpha_numeric(const char c);
      bool is_alpha(const char c) ;

      char peek_next() ;

      void do_number();

      bool is_digit(const char c);

      void do_string();

      char peek() ;

      bool is_at_end();

      void add_token(token_type type) ;
      void add_token(token_type type, const std::string& literal) ;
      char advance() ;

      bool match(const char expected) ;

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
