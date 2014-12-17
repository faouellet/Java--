#ifndef JAVAMM_DRIVER_H
#define JAVAMM_DRIVER_H

#include "node.h"

#include <memory>

namespace javamm {

/// Driver - TODO
class Driver {
public:
  Driver() : Generator{std::make_unique<CodeGenerator>()} {}

  void compile(const std::string &Filename, bool ToAsm);

  void setRoot(ASTNode *N) { ASTRoot = N; }

private:
  std::unique_ptr<CodeGenerator> Generator;
  ASTNode *ASTRoot;
};

} // End namespace javamm

#endif // JAVAMM_DRIVER_H

