#pragma once 

namespace cwt
{
  enum class value_type
  {
    nil = 0, number, string, boolean
  };

  template<typename T>
  struct _model_helper {
    T m_value;

    _model_helper(T const& value) : m_value(value) {}
    value_type type() const noexcept { return value_type::nil; }
    std::string to_string() const noexcept { return "nil"; }
    double number() const { throw std::runtime_error("lox object does not hold a number"); }
    bool boolean() const { throw std::runtime_error("lox object does not hold a bool"); }
    std::string string() const { throw std::runtime_error("lox object does not hold a string"); }
  };

  template<>
  struct _model_helper<bool> {
    bool m_value;

    _model_helper(bool const& value) : m_value(value) {}
    value_type type() const noexcept { return value_type::boolean; }
    std::string to_string() const noexcept { return std::to_string(m_value); }
    double number() const { throw std::runtime_error("lox object does not hold a number"); }
    bool boolean() const { return m_value; }
    std::string string() const { throw std::runtime_error("lox object does not hold a string"); }
  };

  template<>
  struct _model_helper<double> {
    double m_value;

    _model_helper(double const& value) : m_value(value) {}
    value_type type() const noexcept { return value_type::number; }
    std::string to_string() const noexcept { return std::to_string(m_value); }
    double number() const { return m_value; }
    bool boolean() const { throw std::runtime_error("lox object does not hold a bool"); }
    std::string string() const { throw std::runtime_error("lox object does not hold a string"); }
  };

  template<>
  struct _model_helper<std::string> {
    std::string m_value;

    _model_helper(std::string const& value) : m_value(value) {}
    value_type type() const noexcept { return value_type::string; }
    std::string to_string() const noexcept { return m_value; }
    double number() const { throw std::runtime_error("lox object does not hold a number"); }
    bool boolean() const { throw std::runtime_error("lox object does not hold a bool"); }
    std::string string() const { return m_value; }
  };


  class lox_obj 
  {
    public:
 
      
      lox_obj() : m_nil(true) {}

      template <typename T, typename std::enable_if_t<std::is_same_v<T, bool>>* = nullptr>
      lox_obj(T value) : m_nil(false)
      {
          m_value = std::make_unique<_model<bool>>(std::move(value));
      }

      template <typename T, typename std::enable_if_t<std::is_same_v<typename std::decay<T>::type, std::string>>* = nullptr>
      lox_obj(T value) : m_nil(false)
      {
          m_value = std::make_unique<_model<std::string>>(std::move(std::string{value}));
      }

      template <typename T, typename std::enable_if_t<std::is_same_v<T, const char*>>* = nullptr>
      lox_obj(T value) : m_nil(false)
      {
          m_value = std::make_unique<_model<std::string>>(std::move(std::string{value}));
      }

      template <typename T, typename std::enable_if_t<std::is_arithmetic_v<T> && !std::is_same_v<T, bool>>* = nullptr>
      lox_obj(T value) : m_nil(false)
      {
        m_value = std::make_unique<_model<double>>(std::move(value));
      }

      value_type type() const noexcept 
      { 
        if (m_value) 
        {
          return m_value->type(); 
        }
        else 
        {
          return value_type::nil;
        }
      };
      double number() const
      {
        return m_value->number();
      }
      std::string string() const
      {
        return m_value->string();
      }
      bool boolean() const 
      {
        return m_value->boolean();
      }   
      bool nil() const
      {
        return m_nil;
      }
      std::string to_string() const
      {
        return m_value->to_string();
      }

  private:   

      struct _concept {
          virtual ~_concept() {}
          virtual value_type type() const noexcept { return value_type::nil; };
          virtual std::string to_string() const noexcept { return "nil"; };
          virtual double number() const { throw std::runtime_error("lox object does not hold a number"); };
          virtual bool boolean() const { throw std::runtime_error("lox object does not hold a bool"); };
          virtual std::string string() const { throw std::runtime_error("lox object does not hold a string"); };
      };

      template<typename T>
      struct _model : public _concept
      {
        _model_helper<T> helper;

        _model(T const& value) : helper(value) {}
        value_type type() const noexcept override { return helper.type(); }
        std::string to_string() const noexcept override { return helper.to_string(); }
        double number() const override { return helper.number(); }
        bool boolean() const override { return helper.boolean(); }
        std::string string() const override { return helper.string(); }
      };

    private:
      bool m_nil;
      std::unique_ptr<_concept> m_value;
  };

  lox_obj create_another(const lox_obj& old) 
  {
    switch (old.type())
    {
    case value_type::boolean: return old.boolean();
    break; case value_type::number: return old.number();
    break; case value_type::string: return old.string();
    default: return lox_obj(); // creates nil 
    }
  }

} // namespace cwt
