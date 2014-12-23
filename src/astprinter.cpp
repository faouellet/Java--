#include "astprinter.h"
#include "node.h"

#include <cassert>
#include <iomanip>
#include <iostream>

using namespace javamm;

//===----------------------------------------------------------------------===//
// Implementation of the core language printing
//

void ASTPrinter::printConstant(double Val, unsigned Depth) const {
  std::cout << std::setfill(FILL_CHAR) << std::setw(Depth) << "Number: " << Val
            << std::endl;
}

void ASTPrinter::printVariable(const std::string &Val, unsigned Depth) const {
  std::cout << std::setfill(FILL_CHAR) << std::setw(Depth) << "Variable: " << Val
            << std::endl;
}

void ASTPrinter::printBinOp(char Op, ExprNode *LHS, ExprNode *RHS, unsigned Depth) const {
  assert(RHS != nullptr && "BinOp: rhs operand null");
  assert(LHS != nullptr && "BinOp: lhs operand null");

  std::cout << std::setfill(FILL_CHAR) << std::setw(Depth) << "BinOp: " << Op
            << std::endl;

  LHS->print(this, Depth + FILL_WIDTH);
  RHS->print(this, Depth + FILL_WIDTH);
}

void ASTPrinter::printCall(const std::string &FuncName,
                           const std::vector<ExprNode *> Args,
                           unsigned Depth) const {
  std::cout << std::setfill(FILL_CHAR) << std::setw(Depth)
            << "Call: " << FuncName << std::endl;

  for (auto &&EN : Args)
    EN->print(this, Depth + FILL_WIDTH);
}

void ASTPrinter::printPrototype(const std::string &FuncName,
                                const std::vector<std::string> Args,
                                unsigned Depth) const {
  std::cout << std::setfill(FILL_CHAR) << std::setw(Depth)
            << "Prototype: " << FuncName << std::endl;

  for (auto &&Arg : Args)
    std::cout << std::setfill(FILL_CHAR) << std::setw(Depth + FILL_WIDTH)
              << "Arg: " << Arg << std::endl;
}

void ASTPrinter::printFunction(PrototypeNode *Prototype, ExprNode *Body,
                               unsigned Depth) const {
  assert(Prototype != nullptr && "Function: prototype null");
  assert(Body != nullptr && "Function: body null");

  std::cout << std::setfill(FILL_CHAR) << std::setw(Depth)
            << "Function: " << std::endl;

  Prototype->print(this, Depth + FILL_WIDTH);
  Body->print(this, Depth + FILL_WIDTH);
}

//===----------------------------------------------------------------------===//
// Implementation of the control flow printing
//

void ASTPrinter::printIf(ExprNode *Cond, ExprNode *Then, ExprNode *Else,
                         unsigned Depth) const {
  assert(Cond != nullptr && "If: condition is null");
  assert(Then != nullptr && "If: then block is null");
  assert(Else != nullptr && "If: else block is null");

  std::cout << std::setfill(FILL_CHAR) << std::setw(Depth)
            << "If: " << std::endl;

  Cond->print(this, Depth + FILL_WIDTH);
  Then->print(this, Depth + FILL_WIDTH);
  Else->print(this, Depth + FILL_WIDTH);
}

void ASTPrinter::printFor(const std::string &VarName, ExprNode *Begin,
                          ExprNode *End, ExprNode *Step, ExprNode *Body,
                          unsigned Depth) const {

  std::cout << std::setfill(FILL_CHAR) << std::setw(Depth)
            << "For: " << std::endl;
  std::cout << std::setfill(FILL_CHAR) << std::setw(Depth)
            << "Induction Variable: " << VarName << std::endl;

  Begin->print(this, Depth + FILL_WIDTH);
  End->print(this, Depth + FILL_WIDTH);
  Step->print(this, Depth + FILL_WIDTH);
  Body->print(this, Depth + FILL_WIDTH);
}

