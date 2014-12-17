#ifndef JAVAMM_NODE_H
#define JAVAMM_NODE_H

#include "codegenerator.h"

#include <string>
#include <vector>

namespace javamm {

// Base class for all expression nodes
class ExprNode {
public:
  virtual ~ExprNode() {}
  // virtual llvm::Value *codegen(CodeGenerator *) = 0;
};

// Expression class for numeric literal like "1.0"
class NumberExprNode : public ExprNode {
public:
  NumberExprNode(double Num) : Val{Num} {}
  virtual ~NumberExprNode() {}
  // virtual llvm::Value *codegen(CodeGenerator *CodeGen) override;

private:
  double Val;
};

// Expression class for referencing a variable like "MyVar"
class VariableExprNode : public ExprNode {
public:
  VariableExprNode(const std::string &Val) : Name{Val} {}
  virtual ~VariableExprNode() {}
  // virtual llvm::Value *codegen(CodeGenerator *CodeGen) override;

private:
  std::string Name;
};

// Expression class for a binary operator
class BinaryExprNode : public ExprNode {
public:
  BinaryExprNode(char, ExprNode *, ExprNode *) {}
  virtual ~BinaryExprNode() {}
  // virtual llvm::Value *codegen(CodeGenerator *CodeGen) override;
};

// Expression class for function calls
class CallExprNode : public ExprNode {
public:
  CallExprNode(const std::string &FuncName,
               const std::vector<ExprNode *> Arguments)
      : Callee{FuncName}, Args{Arguments} {}
  virtual ~CallExprNode() {}
  // virtual llvm::Value *codegen(CodeGenerator *CodeGen) override;

private:
  std::string Callee;
  std::vector<ExprNode *> Args;
};

// Class representing a function prototype. It has the knowledge of the function
// name, the name of its arguments and, implicitly, the number of arguments the
// function has.
class PrototypeNode {
public:
  PrototypeNode(const std::string &Name, const std::vector<std::string> &Args)
      : FuncName{Name}, ArgsNames{Args} {}
  // llvm::Value *codegen(CodeGenerator *CodeGen);

private:
  std::string FuncName;
  std::vector<std::string> ArgsNames;
};

// Class representing a function definition
class FunctionNode {
public:
  FunctionNode(PrototypeNode *, ExprNode *) {}
  // llvm::Value *codegen(CodeGenerator *CodeGen);
};

} // End namespace javamm

#endif // JAVAMM_NODE_H

