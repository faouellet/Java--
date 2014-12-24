#ifndef JAVAMM_NODE_H
#define JAVAMM_NODE_H

#include <string>
#include <vector>

namespace javamm {

class ASTPrinter;
class CodeGenerator;

// Base class for all nodes in the AST
class ASTNode {
public:
  virtual ~ASTNode() {}
  virtual void codegen(CodeGenerator *) const = 0;
  virtual void print(const ASTPrinter *, unsigned) const = 0;
};

// Base class for all expression nodes
class ExprNode : public ASTNode {
public:
  virtual ~ExprNode() {}
};

// Expression class for variable declaration
class DeclNode : public ExprNode {
public:
  DeclNode(const std::string &Variable, ExprNode *DeclBody)
      : VarName{Variable}, Body{DeclBody} {}
  virtual ~DeclNode() {}
  void codegen(CodeGenerator *CodeGen) const override;
  void print(const ASTPrinter *Printer, unsigned Depth) const override;

private:
  std::string VarName;
  ExprNode *Body;
};

// Expression class for numeric literal like "1.0"
class NumberExprNode : public ExprNode {
public:
  NumberExprNode(double Num) : Val{Num} {}
  virtual ~NumberExprNode() {}
  void codegen(CodeGenerator *CodeGen) const override;
  void print(const ASTPrinter *Printer, unsigned Depth) const override;

private:
  double Val;
};

// Expression class for referencing a variable like "MyVar"
class VariableExprNode : public ExprNode {
public:
  VariableExprNode(const std::string &Val) : Name{Val} {}
  virtual ~VariableExprNode() {}
  void codegen(CodeGenerator *CodeGen) const override;
  void print(const ASTPrinter *Printer, unsigned Depth) const override;
  const std::string &getName() const { return Name; }

private:
  std::string Name;
};

// Expression class for a binary operator
class BinaryExprNode : public ExprNode {
public:
  BinaryExprNode(char Op, ExprNode *LHS, ExprNode *RHS)
      : Operator{Op}, LHSNode{LHS}, RHSNode{RHS} {}
  virtual ~BinaryExprNode() {}
  void codegen(CodeGenerator *CodeGen) const override;
  void print(const ASTPrinter *Printer, unsigned Depth) const override;

private:
  char Operator;
  ExprNode *LHSNode;
  ExprNode *RHSNode;
};

// Expression class for function calls
class CallExprNode : public ExprNode {
public:
  CallExprNode(const std::string &FuncName,
               const std::vector<ExprNode *> Arguments)
      : Callee{FuncName}, Args{Arguments} {}
  virtual ~CallExprNode() {}
  void codegen(CodeGenerator *CodeGen) const override;
  void print(const ASTPrinter *Printer, unsigned Depth) const override;

private:
  std::string Callee;
  std::vector<ExprNode *> Args;
};

// Expression class for for loop
class ForNode : public ExprNode {
public:
  ForNode(const std::string &VarName, ExprNode *Begin, ExprNode *End,
          ExprNode *Step, ExprNode *Body)
      : InductionVariableName{VarName}, BeginNode{Begin}, EndNode{End},
        StepNode{Step}, BodyNode{Body} {}
  virtual ~ForNode() {}
  void codegen(CodeGenerator *CodeGen) const override;
  void print(const ASTPrinter *Printer, unsigned Depth) const override;

private:
  std::string InductionVariableName;
  ExprNode *BeginNode;
  ExprNode *EndNode;
  ExprNode *StepNode;
  ExprNode *BodyNode;
};

// Expression class for if/then/else
class IfNode : public ExprNode {
public:
  IfNode(ExprNode *CondNode, ExprNode *ThenNode, ExprNode *ElseNode)
      : Cond{CondNode}, Then{ThenNode}, Else{ElseNode} {}
  virtual ~IfNode() {}
  void codegen(CodeGenerator *CodeGen) const override;
  void print(const ASTPrinter *Printer, unsigned Depth) const override;

private:
  ExprNode *Cond;
  ExprNode *Then;
  ExprNode *Else;
};

// Class representing a function prototype. It has the knowledge of the function
// name, the name of its arguments and, implicitly, the number of arguments the
// function has.
class PrototypeNode : public ASTNode {
public:
  PrototypeNode(const std::string &Name, const std::vector<std::string> &Args)
      : FuncName{Name}, ArgsNames{Args} {}
  void codegen(CodeGenerator *CodeGen) const override;
  void print(const ASTPrinter *Printer, unsigned Depth) const override;
  const std::vector<std::string> &getArgsNames() const { return ArgsNames; }

private:
  std::string FuncName;
  std::vector<std::string> ArgsNames;
};

// Class representing a function definition
class FunctionNode : public ASTNode {
public:
  FunctionNode(PrototypeNode *FuncProto, ExprNode *FuncBody)
      : Prototype{FuncProto}, Body{FuncBody} {}
  void codegen(CodeGenerator *CodeGen) const override;
  void print(const ASTPrinter *Printer, unsigned Depth) const override;

private:
  PrototypeNode *Prototype;
  ExprNode *Body;
};

} // End namespace javamm

#endif // JAVAMM_NODE_H

