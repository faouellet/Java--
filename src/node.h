//===------------------ node.h - Node class definition --------------------===//
/// \file
///
/// This file contains the declarations of all the node classes that can appear
/// in a java-- AST.
///
//===----------------------------------------------------------------------===//

#ifndef JAVAMM_NODE_H
#define JAVAMM_NODE_H

#include "llvm/Support/Casting.h"

#include <string>
#include <vector>

namespace javamm {

class ASTPrinter;
class CodeGenerator;

/// \brief ASTNode
/// Base class for all nodes in the AST.
class ASTNode {
public:
  enum NodeKind {
    DECL,
    NUMBER_EXPR,
    VARIABLE_EXPR,
    BINARY_EXPR,
    CALL_EXPR,
    FUNCTION,
    PROTOTYPE,
    IF,
    FOR,
    WHILE,
    IO
  };

public:
  ASTNode(NodeKind K) : Kind{K} {};
  virtual ~ASTNode() {}
  virtual void codegen(CodeGenerator *) const = 0;
  virtual void print(const ASTPrinter *, unsigned) const = 0;

public:
  NodeKind getKind() const { return Kind; }

private:
  NodeKind Kind;
};

/// \brief ExprNode
/// Base class for all expression nodes i.e. nodes that can produce a value.
class ExprNode : public ASTNode {
public:
  ExprNode(NodeKind K) : ASTNode{K} {}
  virtual ~ExprNode() {}
};

/// \brief DeclNode
/// Expression class for variable declaration.
/// A correct variable declaration will look like this: 
///             var X = 42;
class DeclNode : public ExprNode {
public:
  DeclNode(const std::string &Variable, ExprNode *DeclBody)
      : ExprNode{DECL}, VarName{Variable}, Body{DeclBody} {}
  virtual ~DeclNode() {}
  void codegen(CodeGenerator *CodeGen) const override;
  void print(const ASTPrinter *Printer, unsigned Depth) const override;

public:
  static bool classof(const ExprNode *EN) { return EN->getKind() == DECL; }

private:
  std::string VarName;
  ExprNode *Body;
};

/// \brief NumberExprNode
/// Expression class for numeric literal like "1.0"
class NumberExprNode : public ExprNode {
public:
  NumberExprNode(double Num) : ExprNode{NUMBER_EXPR}, Val{Num} {}
  virtual ~NumberExprNode() {}
  void codegen(CodeGenerator *CodeGen) const override;
  void print(const ASTPrinter *Printer, unsigned Depth) const override;

public:
  static bool classof(const ExprNode *EN) {
    return EN->getKind() == NUMBER_EXPR;
  }

private:
  double Val;
};

/// \brief VariableExprNode
/// Expression class for referencing a variable like "MyVar"
class VariableExprNode : public ExprNode {
public:
  VariableExprNode(const std::string &Val)
      : ExprNode{VARIABLE_EXPR}, Name{Val} {}
  virtual ~VariableExprNode() {}
  void codegen(CodeGenerator *CodeGen) const override;
  void print(const ASTPrinter *Printer, unsigned Depth) const override;
  const std::string &getName() const { return Name; }

public:
  static bool classof(const ExprNode *EN) {
    return EN->getKind() == VARIABLE_EXPR;
  }

private:
  std::string Name;
};

/// \brief BinaryExprNode
/// Expression class for a binary operator.
/// The currently accepted binary operators are:
/// '=', '+', '-', '*', '/' 
class BinaryExprNode : public ExprNode {
public:
  BinaryExprNode(char Op, ExprNode *LHS, ExprNode *RHS)
      : ExprNode{BINARY_EXPR}, Operator{Op}, LHSNode{LHS}, RHSNode{RHS} {}
  virtual ~BinaryExprNode() {}
  void codegen(CodeGenerator *CodeGen) const override;
  void print(const ASTPrinter *Printer, unsigned Depth) const override;

public:
  static bool classof(const ExprNode *EN) {
    return EN->getKind() == BINARY_EXPR;
  }

private:
  char Operator;
  ExprNode *LHSNode;
  ExprNode *RHSNode;
};

/// \brief CallExprNode
/// Expression class for function calls
class CallExprNode : public ExprNode {
public:
  CallExprNode(const std::string &FuncName,
               const std::vector<ExprNode *> Arguments)
      : ExprNode{CALL_EXPR}, Callee{FuncName}, Args{Arguments} {}
  virtual ~CallExprNode() {}
  void codegen(CodeGenerator *CodeGen) const override;
  void print(const ASTPrinter *Printer, unsigned Depth) const override;

public:
  static bool classof(const ExprNode *EN) { return EN->getKind() == CALL_EXPR; }

private:
  std::string Callee;
  std::vector<ExprNode *> Args;
};

/// \brief WhileNode
/// Expression class for a while loop
/// A while loop looks the same as one in C/C++
/// while(cond) { stmts }
class WhileNode : public ExprNode {
public:
  WhileNode(ExprNode *CondNode, ExprNode *BodyNode)
      : ExprNode{WHILE}, Cond{CondNode}, Body{BodyNode} {}
  virtual ~WhileNode() {}
  void codegen(CodeGenerator *CodeGen) const override;
  void print(const ASTPrinter *Printer, unsigned Depth) const override;

public:
  static bool classof(const ExprNode *EN) { return EN->getKind() == WHILE; }

private:
  ExprNode *Cond;
  ExprNode *Body;
};

/// \brief ForNode
/// Expression class for for loop.
/// A for loop looks the same as one in C/C++:
/// for(init; cond; step) { stmts }
class ForNode : public ExprNode {
public:
  ForNode(const std::string &VarName, ExprNode *BeginNode, ExprNode *EndNode,
          ExprNode *StepNode, ExprNode *BodyNode)
      : ExprNode{FOR}, InductionVariableName{VarName}, Begin{BeginNode},
        End{EndNode}, Step{StepNode}, Body{BodyNode} {}
  virtual ~ForNode() {}
  void codegen(CodeGenerator *CodeGen) const override;
  void print(const ASTPrinter *Printer, unsigned Depth) const override;

public:
  static bool classof(const ExprNode *EN) { return EN->getKind() == FOR; }

private:
  std::string InductionVariableName;
  ExprNode *Begin;
  ExprNode *End;
  ExprNode *Step;
  ExprNode *Body;
};

/// \brief IfNode
/// Expression class for if/then/else
/// A if condition requires both a true branch and false branch like in the
/// following example:
/// if(cond) then { stmts } else { stmts }
class IfNode : public ExprNode {
public:
  IfNode(ExprNode *CondNode, ExprNode *ThenNode, ExprNode *ElseNode)
      : ExprNode{IF}, Cond{CondNode}, Then{ThenNode}, Else{ElseNode} {}
  virtual ~IfNode() {}
  void codegen(CodeGenerator *CodeGen) const override;
  void print(const ASTPrinter *Printer, unsigned Depth) const override;

public:
  static bool classof(const ExprNode *EN) { return EN->getKind() == IF; }

private:
  ExprNode *Cond;
  ExprNode *Then;
  ExprNode *Else;
};

/// \brief PrototypeNode
/// Class representing a function prototype. It has the knowledge of the function
/// name, the name of its arguments and, implicitly, the number of arguments the
/// function has. 
class PrototypeNode : public ASTNode {
public:
  PrototypeNode(const std::string &Name, const std::vector<std::string> &Args)
      : ASTNode{PROTOTYPE}, FuncName{Name}, ArgsNames{Args} {}
  void codegen(CodeGenerator *CodeGen) const override;
  void print(const ASTPrinter *Printer, unsigned Depth) const override;
  const std::vector<std::string> &getArgsNames() const { return ArgsNames; }

public:
  static bool classof(const ExprNode *EN) { return EN->getKind() == PROTOTYPE; }

private:
  std::string FuncName;
  std::vector<std::string> ArgsNames;
};

/// \brief FunctionNode
/// Class representing a function definition.
/// A function definition starts with the "def" keyword like this:
/// def foo(x,y) { stmts }
class FunctionNode : public ASTNode {
public:
  FunctionNode(PrototypeNode *FuncProto, ExprNode *FuncBody)
      : ASTNode{FUNCTION}, Prototype{FuncProto}, Body{FuncBody} {}
  void codegen(CodeGenerator *CodeGen) const override;
  void print(const ASTPrinter *Printer, unsigned Depth) const override;

public:
  static bool classof(const ExprNode *EN) { return EN->getKind() == FUNCTION; }

private:
  PrototypeNode *Prototype;
  ExprNode *Body;
};

/// \brief IONode
/// Class representing an input action
/// An input action starts with the "println" keyword followed by a string like
/// this:
/// println "Hello World"
class IONode : public ASTNode {
public:
  IONode(const std::string &S) : ASTNode{IO}, Message{S} {}
  void codegen(CodeGenerator *CodeGen) const override;
  void print(const ASTPrinter *Printer, unsigned Depth) const override;

public:
  static bool classof(const ExprNode *EN) { return EN->getKind() == IO; }

private:
  std::string Message;
};

} // End namespace javamm

#endif // JAVAMM_NODE_H

