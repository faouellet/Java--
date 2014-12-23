#include "driver.h"
#include "lexer.h"
#include "parser.hpp"

#include <fstream>

using namespace javamm;

void Driver::compile(const std::string &Filename, bool ToAsm) {
  std::ifstream FileStream(Filename);
  if (!FileStream.is_open())
    return;

  Lexer TheLexer(&FileStream);
  Parser TheParser(TheLexer, *this);

  TheParser.parse();

  if (ASTRoot) {
    ASTRoot->print(Printer.get(), 0);
    //ASTRoot->codegen(Generator.get());
    //if (!ToAsm)
    //  Generator->dumpIR();
  }
}

