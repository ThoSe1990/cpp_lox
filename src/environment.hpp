#pragma once

namespace cwt
{
  class environment
  {
    public:
      void define(const std::string& name, const lox_obj& value)
      {
        // this allows redefinition of variables, 
        // add check if var already exists here ... 
        m_data[name] = create_another(value);
      }
      lox_obj& get(token t)
      {
        if (m_data.count(t.lexeme))
        {
          return m_data[t.lexeme];
        }
        std::string s{"Undefined variable \'"};
        s.append(t.lexeme);
        s.append("\'.");
        runtime_error(t, s);
      }
    private:
    private:
      std::unordered_map<std::string, lox_obj> m_data;
  };
} // namespace cwt
