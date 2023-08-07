#pragma once

namespace cwt
{
  class environment
  {
    public:

      void set_enclosing(environment* env) 
      {
        m_enclosing = env;
      }

      void define(const std::string& name, const lox_obj& value)
      {
        // this allows redefinition of variables, 
        // add check if var already exists here ... 
        m_data[name] = create_another(value);
      }
      void assign(token name, const lox_obj& value)
      {
        if (m_data.count(name.lexeme))
        {
          m_data[name.lexeme] = create_another(value);
        }
        else if (m_enclosing)
        {
          m_enclosing->assign(name, value);
        }
        else 
        {
          std::string s{"Undefined variable \'"};
          s.append(name.lexeme);
          s.append("\'.");
          runtime_error(name, s);
        }
      }
      lox_obj& get(token t)
      {
        if (m_data.count(t.lexeme))
        {
          return m_data[t.lexeme];
        }
        if (m_enclosing) 
        {
          return m_enclosing->get(t);
        }
        std::string s{"Undefined variable \'"};
        s.append(t.lexeme);
        s.append("\'.");
        runtime_error(t, s);
      }
    private:
      std::unordered_map<std::string, lox_obj> m_data;
      environment* m_enclosing;
  };
} // namespace cwt
