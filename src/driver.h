//===----------------- driver.h - Driver class definition -----------------===//
/// \file
///
/// This file contains the compiler driver.
///
//===----------------------------------------------------------------------===//

#ifndef JAVAMM_DRIVER_H
#define JAVAMM_DRIVER_H

#include "astprinter.h"
#include "codegenerator.h"
#include "node.h"

#include <memory>

namespace javamm {

/// \brief Driver
/// Implementation of a basic driver that manages the actions to perform on the
/// AST. It can starts both the compilation and the printing process.
class Driver {
public:
  Driver()
      : Printer{std::make_unique<ASTPrinter>()},
        Generator{std::make_unique<CodeGenerator>()}, ASTRoot(nullptr) {}

  void compile(int NbArgs, char **Args);

  void setRoot(ASTNode *N) { ASTRoot = N; }

private:
  std::unique_ptr<ASTPrinter> Printer;
  std::unique_ptr<CodeGenerator> Generator;
  ASTNode *ASTRoot;
};

} // End namespace javamm

#endif // JAVAMM_DRIVER_H

