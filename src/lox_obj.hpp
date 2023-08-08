#pragma once 

#include <type_traits>
#include <string>
#include <memory>
#include <stdexcept>

#include "lox_function.hpp"

namespace cwt
{
  enum class value_type
  {
    nil = 0, number, string, boolean, callable
  };

  
  template<typename T>
  struct _model_helper 
  {
    T m_value;

    _model_helper(const T& value) : m_value(value) {}
    value_type type() const noexcept { return value_type::nil; }
    std::string to_string() const noexcept { return "nil"; }
    double number() const { throw std::runtime_error("lox object does not hold a number"); }
    bool boolean() const { throw std::runtime_error("lox object does not hold a bool"); }
    lox_function callable() const { throw std::runtime_error("lox object does not hold a function"); }
    std::string string() const { throw std::runtime_error("lox object does not hold a string"); }
  };
  
  class lox_obj 
  {
    public:
      lox_obj();

      template <typename T, typename std::enable_if_t<std::is_same_v<T, lox_function>>* = nullptr>
      lox_obj(T value);

      template <typename T, typename std::enable_if_t<std::is_same_v<T, bool>>* = nullptr>
      lox_obj(T value);

      template <typename T, typename std::enable_if_t<std::is_same_v<typename std::decay<T>::type, std::string>>* = nullptr>
      lox_obj(T value);

      template <typename T, typename std::enable_if_t<std::is_same_v<T, const char*>>* = nullptr>
      lox_obj(T value);

      template <typename T, typename std::enable_if_t<std::is_arithmetic_v<T> && !std::is_same_v<T, bool>>* = nullptr>
      lox_obj(T value);

      value_type type() const noexcept;
      double number() const;
      std::string string() const;
      bool boolean() const;
      lox_function callable() const;
      bool nil() const;
      std::string to_string() const;

  private:   
      struct _concept {
          virtual ~_concept() {}
          virtual value_type type() const noexcept { return value_type::nil; };
          virtual std::string to_string() const noexcept { return "nil"; };
          virtual double number() const { throw std::runtime_error("lox object does not hold a number"); };
          virtual bool boolean() const { throw std::runtime_error("lox object does not hold a bool"); };
          virtual lox_function callable() const { throw std::runtime_error("lox object does not hold a function"); };
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
        lox_function callable() const { return helper.callable(); }
        std::string string() const override { return helper.string(); }
      };

    private:
      bool m_nil;
      std::unique_ptr<_concept> m_value;
  };

  lox_obj create_another(const lox_obj& old) ;

} // namespace cwt
