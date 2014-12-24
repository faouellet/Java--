#ifndef JAVAMM_AST_PRINTER_H
#define JAVAMM_AST_PRINTER_H

#include <string>
#include <vector>

namespace javamm {

class ExprNode;
class PrototypeNode;

class ASTPrinter {
public: // Core language nodes printing
  void printConstant(double Val, unsigned Depth) const;
  void printDecl(const std::string &Var, ExprNode *Body, unsigned Depth) const;
  void printVariable(const std::string &Val, unsigned Depth) const;
  void printBinOp(char Op, ExprNode *LHS, ExprNode *RHS, unsigned Depth) const;
  void printCall(const std::string &FuncName,
                 const std::vector<ExprNode *> Args, unsigned Depth) const;
  void printPrototype(const std::string &FuncName,
                      const std::vector<std::string> Args,
                      unsigned Depth) const;
  void printFunction(PrototypeNode *Prototype, ExprNode *Body,
                     unsigned Depth) const;

public: // Control flow nodes printing
  void printIf(ExprNode *Cond, ExprNode *Then, ExprNode *Else,
               unsigned Depth) const;
  void printFor(const std::string &VarName, ExprNode *Begin, ExprNode *End,
                ExprNode *Step, ExprNode *Body, unsigned Depth) const;

private:
  const char FILL_CHAR = ' ';
  const char FILL_WIDTH = 4;
};

} // End namespace javamm

#endif // JAVAMM_AST_PRINTER_H
