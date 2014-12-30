#include "node.h"

#include "astprinter.h"
#include "codegenerator.h"

using namespace javamm;
using llvm::Function;
using llvm::Value;

void DeclNode::codegen(CodeGenerator *CodeGen) const {
  CodeGen->genDecl(VarName, Body);
}

void DeclNode::print(const ASTPrinter *Printer, unsigned Depth) const {
  Printer->printDecl(VarName, Body, Depth);
}

void NumberExprNode::codegen(CodeGenerator *CodeGen) const {
  CodeGen->genConstant(Val);
}

void NumberExprNode::print(const ASTPrinter *Printer, unsigned Depth) const {
  Printer->printConstant(Val, Depth);
}

void VariableExprNode::codegen(CodeGenerator *CodeGen) const {
  CodeGen->genVariable(Name);
}

void VariableExprNode::print(const ASTPrinter *Printer, unsigned Depth) const {
  Printer->printVariable(Name, Depth);
}

void BinaryExprNode::codegen(CodeGenerator *CodeGen) const {
  CodeGen->genBinOp(Operator, LHSNode, RHSNode);
}

void BinaryExprNode::print(const ASTPrinter *Printer, unsigned Depth) const {
  Printer->printBinOp(Operator, LHSNode, RHSNode, Depth);
}

void CallExprNode::codegen(CodeGenerator *CodeGen) const {
  CodeGen->genCall(Callee, Args);
}

void CallExprNode::print(const ASTPrinter *Printer, unsigned Depth) const {
  Printer->printCall(Callee, Args, Depth);
}

void WhileNode::codegen(CodeGenerator *CodeGen) const {
  CodeGen->genWhile(Cond, Body);
}

void WhileNode::print(const ASTPrinter *Printer, unsigned Depth) const {
  Printer->printWhile(Cond, Body, Depth);
}

void ForNode::codegen(CodeGenerator *CodeGen) const {
  CodeGen->genFor(InductionVariableName, Begin, End, Step, Body);
}

void ForNode::print(const ASTPrinter *Printer, unsigned Depth) const {
  Printer->printFor(InductionVariableName, Begin, End, Step, Body, Depth);
}

void IfNode::codegen(CodeGenerator *CodeGen) const {
  CodeGen->genIf(Cond, Then, Else);
}

void IfNode::print(const ASTPrinter *Printer, unsigned Depth) const {
  Printer->printIf(Cond, Then, Else, Depth);
}

void PrototypeNode::codegen(CodeGenerator *CodeGen) const {
  CodeGen->genPrototype(FuncName, ArgsNames);
}

void PrototypeNode::print(const ASTPrinter *Printer, unsigned Depth) const {
  Printer->printPrototype(FuncName, ArgsNames, Depth);
}

void FunctionNode::codegen(CodeGenerator *CodeGen) const {
  CodeGen->genFunction(Prototype, Body);
}

void FunctionNode::print(const ASTPrinter *Printer, unsigned Depth) const {
  Printer->printFunction(Prototype, Body, Depth);
}

