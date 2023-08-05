#pragma once 

namespace cwt
{
  class parser
  {
    public: 
      parser(const std::vector<token>& tokens) : m_tokens(tokens) {}

      expression* parse() 
      {

      }

    private:


    private:
      std::size_t m_current{0};
      std::vector<token> m_tokens;
  };
} // namespace cwt
