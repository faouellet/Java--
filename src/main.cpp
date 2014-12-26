#include "driver.h"

#include <iostream>

/// main - TODO
// This will deal with the compiler options
int main(int argc, char **argv) {
  if (argc != 2) {
    std::cerr << "Incorrect number of arguments" << std::endl;
    std::cerr << "Usage: javamm <filename>" << std::endl;
    return 1;
  }

  javamm::Driver Compiler;

  Compiler.compile(argc, argv);

  std::cout << "Compilation done" << std::endl;
}

