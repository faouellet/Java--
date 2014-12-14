#ifndef NODE_H
#define NODE_H

#include <string>
#include <vector>

namespace javamm {

// Base class for all expression nodes
class ExprNode {
public:
  virtual ~ExprNode() {}
};

// Expression class for numeric literal like "1.0"
class NumberExprNode : public ExprNode {
public:
  NumberExprNode(double) {}
};

// Expression class for referencing a variable like "MyVar"
class VariableExprNode : public ExprNode {
public:
  VariableExprNode(const std::string &Val) : Name{Val} {}

private:
  std::string Name;
};

// Expression class for a binary operator
class BinaryExprNode : public ExprNode {
public:
  BinaryExprNode(char, ExprNode, ExprNode) {}
};

// Expression class for function calls
class CallExprNode : public ExprNode {
public:
  CallExprNode(const std::string &FuncName,
               const std::vector<ExprNode *> Arguments)
      : Callee{FuncName}, Args{Arguments} {}

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

private:
  std::string FuncName;
  std::vector<std::string> ArgsNames;
};

// Class representing a function definition
class FunctionNode {
public:
  FunctionNode(PrototypeNode, ExprNode) {}
};

} // End namespace javamm

#endif // NODE_H

