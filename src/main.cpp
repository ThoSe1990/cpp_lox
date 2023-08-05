#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <memory>

namespace cwt 
{
  bool has_error = false;

  void report(const std::size_t line, const std::string& where, const std::string& msg)
  {
    std::cerr << "[REPORT] " << where << ':' << line << ": " << msg << '\n';
    has_error = true;
  }

  void error(const std::size_t line, const std::string& msg) 
  {
    report(line, "", msg);
  }
}

#include "token.hpp"
#include "scanner.hpp"

std::string readFile(const std::string& filename) 
{
  std::ifstream file(filename, std::ios::in | std::ios::binary);
  std::stringstream buffer;
  buffer << file.rdbuf(); 
  return buffer.str();
}


void run(const std::string& src) 
{
  cwt::scanner scanner(src);
  std::vector<cwt::token> tokens = scanner.scan_tokens();

  for (const auto& t : tokens){
    std::cout << t.to_string() << '\n';
  }
}

#include "expr.hpp"

int main(int argc, char** argv)
{
  cwt::expression* l1 = new cwt::expr_literal("123");
  cwt::expression* l2 = new cwt::expr_literal("45.67");

  cwt::expression* unary = new cwt::expr_unary(
      cwt::token(cwt::token_type::MINUS, "-", 1),
      l1
    );

  cwt::expression* gr = new cwt::expr_grouping(l2);

  cwt::expr_binary exp(
    unary,
    cwt::token(cwt::token_type::STAR, "*", 1),
    gr
  );

  cwt::printer().print(exp);

  // delete l1; 
  // delete l2;
  // delete unary;
  // delete gr; 

  return 0;
}