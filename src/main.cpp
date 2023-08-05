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


#include "expr.hpp"
#include "parser.hpp"

void run(const std::string& src) 
{
  cwt::scanner scanner(src);
  std::vector<cwt::token> tokens = scanner.scan_tokens();

  cwt::parser<std::string> parser(tokens);
  cwt::expression<std::string>* expr = parser.parse(); 

  if (expr) {
    cwt::printer().print(*expr);
    delete expr;
  }
}


int main(int argc, char** argv)
{
  if (argc == 1) {

  } else if (argc == 2) {
    const std::string path{argv[1]};
    std::cout << "reading: " << path << '\n';
    run(readFile(path));
  } else {
    std::cerr << "invalid argc given\n";
    return -1;  
  }

  std::cout << "program done!\n";
  return 0;
}