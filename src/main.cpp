#include "driver.h"

#include <iostream>

// TODO: Is this still necessary??

/// main - TODO
// This will deal with the compiler options
int main(int argc, char **argv) {
  javamm::Driver Compiler;

  Compiler.compile(argc, argv);

  std::cout << "Compilation done" << std::endl;
}

