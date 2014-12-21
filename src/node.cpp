#include "node.h"
#include "codegenerator.h"

using namespace javamm;
using llvm::Function;
using llvm::Value;

void NumberExprNode::codegen(CodeGenerator *CodeGen) {
  CodeGen->genConstant(Val);
}

void VariableExprNode::codegen(CodeGenerator *CodeGen) {
  CodeGen->genVariable(Name);
}

void BinaryExprNode::codegen(CodeGenerator *CodeGen) {
  CodeGen->genBinOp(Operator, LHSNode, RHSNode);
}

void CallExprNode::codegen(CodeGenerator *CodeGen) {
  CodeGen->genCall(Callee, Args);
}

void IfNode::codegen(CodeGenerator *CodeGen) {
  CodeGen->genIf(Cond, Then, Else); 
}

void PrototypeNode::codegen(CodeGenerator *CodeGen) {
  CodeGen->genPrototype(FuncName, ArgsNames);
}

void FunctionNode::codegen(CodeGenerator *CodeGen) {
  CodeGen->genFunction(Prototype, Body);
}

