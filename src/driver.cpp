#include "driver.h"
#include "lexer.h"
#include "parser.hpp"

#include "llvm/Support/CommandLine.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/raw_ostream.h"

#include <fstream>
#include <iostream>
#include <system_error>

using namespace javamm;

//===----------------------------------------------------------------------===//
// Command line options
//

//static llvm::cl::opt<std::string> InputFilename(llvm::cl::Positional,
//                                                llvm::cl::desc("<input file>"),
//                                                llvm::cl::init("-"));
//
//static llvm::cl::opt<std::string>
//    OutputFilename("o", llvm::cl::desc("Specify output filename"),
//                   llvm::cl::value_desc("filename"));
//
//static llvm::cl::opt<bool> ToAsm("asm",
//                                 llvm::cl::desc("Compile the file to asm"));
//
//static llvm::cl::opt<bool>
//    Dump("dump", llvm::cl::desc("Dump the output on the terminal"));
//
//static llvm::cl::opt<bool>
//    Print("print", llvm::cl::desc("Print the AST on the terminal"));

//===----------------------------------------------------------------------===//
// Implementation of the driver
//

void Driver::compile(int NbArgs, char **Args) {
  llvm::cl::ParseCommandLineOptions(NbArgs, Args);

  std::ifstream FileStream(Args[1]);
  if (!FileStream.is_open()) {
    std::cout << "Couldn't open the java-- file specified" << std::endl;
    return;
  }

  Lexer TheLexer(&FileStream);
  Parser TheParser(TheLexer, *this);

  TheParser.parse();

  if (ASTRoot) {
    // if (Print)
    //  ASTRoot->print(Printer.get(), 0);
    // else
    ASTRoot->codegen(Generator.get());

    // if (Dump)
    Generator->dumpIR();

    // if (!OutputFilename.empty()) {
    //  std::error_code EC;
    //  llvm::raw_fd_ostream OutputStream(OutputFilename, EC,
    //                                    llvm::sys::fs::F_None);

    //  Generator->printIR(OutputStream);
  }
}

