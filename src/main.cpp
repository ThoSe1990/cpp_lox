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
  using expr = cwt::expression<std::string>;

  expr* l1 = new cwt::expr_literal<std::string>("123");
  expr* l2 = new cwt::expr_literal<std::string>("45.67");

  expr* unary = new cwt::expr_unary<std::string>(
      cwt::token(cwt::token_type::MINUS, "-", 1),
      l1
    );

  expr* gr = new cwt::expr_grouping<std::string>(l2);

  cwt::expr_binary<std::string> exp(
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