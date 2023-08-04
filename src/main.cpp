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

#include "ast_generator.hpp"

int main(int argc, char** argv)
{

  cwt::expr_literal l1(std::string{"hello world"});
  cwt::expr_literal l2(std::string{"world"});
  cwt::token t(cwt::token_type::AND, "test", 1);
  cwt::expr_assign ea (t, &l1);
  cwt::expr_binary eb(&l1, t, &l2);
  // std::unique_ptr<cwt::expr> expr =  std::make_unique<cwt::Binary>(
  //   &l1, t, &l2
  // );
  cwt::printer<std::string> p;
  p.print(ea);
  p.print(eb);
  // e.accept(p);

  return 0;
// if (argc == 1) {

  // } else if (argc == 2) {
  //   const std::string path{argv[1]};
  //   std::cout << "reading: " << path << '\n';
  //   run(readFile(path));
  // } else {
  //   std::cerr << "invalid argc given\n";
  //   return -1;  
  // }

}