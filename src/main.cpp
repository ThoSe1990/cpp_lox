#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <memory>
#include <type_traits>
#include <unordered_map>
#include <optional>



std::string readFile(const std::string& filename) 
{
  std::ifstream file(filename, std::ios::in | std::ios::binary);
  std::stringstream buffer;
  buffer << file.rdbuf(); 
  return buffer.str();
}


#include "scanner.hpp"
#include "token.hpp"
#include "lox_obj.hpp"
#include "stmt.hpp"
#include "interpreter.hpp"
#include "parser.hpp"

void run(const std::string& src) 
{
  using namespace cwt; 
  scanner scanner(src);
  std::vector<token> tokens = scanner.scan_tokens();
  
  {
    parser<lox_obj> parser(tokens);
    std::vector<std::unique_ptr<lox_statement<lox_obj>>> statements = parser.parse(); 

    if (statements.empty() == false)
    {
      interpreter().interpret(std::move(statements));
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