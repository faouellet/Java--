#include "parser.hpp"

#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"

#include <fstream>
#include <iostream>

int main(int argc, char **argv) {
  if (argc < 2) {
    std::cerr << "Incorrect number of arguments" << std::endl;
    std::cerr << "Usage: javamm <filename>" << std::endl;
    return 1;
  }

  std::cout << "Hello world" << std::endl;
}

