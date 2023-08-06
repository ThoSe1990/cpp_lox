#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <memory>

#include "token.hpp"

namespace cwt 
{
  void runtime_error(const token& t, const std::string& msg);
  void report(const std::size_t line, const std::string& where, const std::string& msg);
  void error(const std::size_t line, const std::string& msg);
}

#include "lox_obj.hpp"

#include "scanner.hpp"
#include "expr.hpp"
#include "printer.hpp"
#include "parser.hpp"
#include "interpreter.hpp"

namespace cwt 
{
  bool has_error = false;
  bool has_runtime_error = false; 

  void runtime_error(const token& t, const std::string& msg)
  {
    std::string s{t.to_string()};
    s.append(" Operands must be two numbers or two strings.");
    has_runtime_error = true;
    throw std::runtime_error(s);
  }

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

std::string readFile(const std::string& filename) 
{
  std::ifstream file(filename, std::ios::in | std::ios::binary);
  std::stringstream buffer;
  buffer << file.rdbuf(); 
  return buffer.str();
}




void run(const std::string& src) 
{
  using namespace cwt; 
  scanner scanner(src);
  std::vector<token> tokens = scanner.scan_tokens();
  
  {
    parser<std::string> parser(tokens);
    expression<std::string>* expr = parser.parse(); 

    if (expr) {
      printer().print(*expr);
      delete expr;
    }
  }
   std::cout << "\nresults in:\n\n";
  {
    parser<lox_obj> parser(tokens);
    expression<lox_obj>* expr = parser.parse(); 
    if (expr) {
      interpreter().interpret(*expr);
      delete expr;
    } 
  }

}


int main(int argc, char** argv)
{
  if (argc == 1) {

  } else if (argc == 2) {
    const std::string path{argv[1]};
    std::cout << "reading: " << path << "\n\n";
    run(readFile(path));
  } else {
    std::cerr << "invalid argc given\n";
    return -1;  
  }

  std::cout << "\n=====================\n";
  std::cout << "program done!\n";
  return 0;
}