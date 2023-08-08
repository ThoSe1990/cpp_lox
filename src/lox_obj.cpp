#include <stdexcept>

#include "lox_obj.hpp"

namespace cwt
{

  template<>
  struct _model_helper<bool> 
  {
    bool m_value;

    _model_helper(const bool& value) : m_value(value) {}
    value_type type() const noexcept { return value_type::boolean; }
    std::string to_string() const noexcept { return std::to_string(m_value); }
    double number() const { throw std::runtime_error("lox object does not hold a number"); }
    bool boolean() const { return m_value; }
    std::string string() const { throw std::runtime_error("lox object does not hold a string"); }
    lox_function callable() const { throw std::runtime_error("lox object does not hold a function"); }
  };

  template<>
  struct _model_helper<double> 
  {
    double m_value;

    _model_helper(const double& value) : m_value(value) {}
    value_type type() const noexcept { return value_type::number; }
    std::string to_string() const noexcept { return std::to_string(m_value); }
    double number() const { return m_value; }
    bool boolean() const { throw std::runtime_error("lox object does not hold a bool"); }
    std::string string() const { throw std::runtime_error("lox object does not hold a string"); }
    lox_function callable() const { throw std::runtime_error("lox object does not hold a function"); }
  };

  template<>
  struct _model_helper<std::string> 
  {
    std::string m_value;

    _model_helper(const std::string& value) : m_value(value) {}
    value_type type() const noexcept { return value_type::string; }
    std::string to_string() const noexcept { return m_value; }
    double number() const { throw std::runtime_error("lox object does not hold a number"); }
    bool boolean() const { throw std::runtime_error("lox object does not hold a bool"); }
    std::string string() const { return m_value; }
    lox_function callable() const { throw std::runtime_error("lox object does not hold a function"); }
  };

  
  template<>
  struct _model_helper<lox_function> 
  {
    lox_function m_value;

    _model_helper(lox_function value) : m_value(value) {}
    value_type type() const noexcept { return value_type::callable; }
    std::string to_string() const noexcept { return "lox_function ..."; }
    double number() const { throw std::runtime_error("lox object does not hold a number"); }
    bool boolean() const { throw std::runtime_error("lox object does not hold a bool"); }
    std::string string() const { throw std::runtime_error("lox object does not hold a string"); }
    lox_function callable() const { return m_value; }
  };


  lox_obj::lox_obj() : m_nil(true) {}

  template <typename T, typename std::enable_if_t<std::is_same_v<T, lox_function>>* = nullptr>
  lox_obj::lox_obj(T value) : m_nil(false)
  {
    m_value = std::make_unique<_model<T>>(std::move(value));
  }

  template <typename T, typename std::enable_if_t<std::is_same_v<T, bool>>* = nullptr>
  lox_obj::lox_obj(T value) : m_nil(false)
  {
    m_value = std::make_unique<_model<bool>>(std::move(value));
  }

  template <typename T, typename std::enable_if_t<std::is_same_v<typename std::decay<T>::type, std::string>>* = nullptr>
  lox_obj::lox_obj(T value) : m_nil(false)
  {
    m_value = std::make_unique<_model<std::string>>(std::move(std::string{value}));
  }

  template <typename T, typename std::enable_if_t<std::is_same_v<T, const char*>>* = nullptr>
  lox_obj::lox_obj(T value) : m_nil(false)
  {
    m_value = std::make_unique<_model<std::string>>(std::move(std::string{value}));
  }

  template <typename T, typename std::enable_if_t<std::is_arithmetic_v<T> && !std::is_same_v<T, bool>>* = nullptr>
  lox_obj::lox_obj(T value) : m_nil(false)
  {
    m_value = std::make_unique<_model<double>>(std::move(value));
  }

  value_type lox_obj::type() const noexcept 
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
  double lox_obj::number() const
  {
    return m_value->number();
  }
  std::string lox_obj::string() const
  {
    return m_value->string();
  }
  bool lox_obj::boolean() const 
  {
    return m_value->boolean();
  }   
  bool lox_obj::nil() const
  {
    return m_nil;
  }
  lox_function lox_obj::callable() const
  {
    return m_value->callable();
  }
  std::string lox_obj::to_string() const
  {
    return m_value->to_string();
  }




lox_obj create_another(const lox_obj& old) 
{
  switch (old.type())
  {
  case value_type::boolean: return old.boolean();
  break; case value_type::number: return old.number();
  break; case value_type::string: return old.string();
  break; case value_type::callable: return old.callable();
  default: return lox_obj(); // creates nil 
  }
}

} // namespace cwt
