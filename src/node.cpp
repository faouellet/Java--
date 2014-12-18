#include "node.h"

using namespace javamm;
using llvm::Function;
using llvm::Value;

Value *NumberExprNode::codegen(CodeGenerator *CodeGen) {
  return CodeGen->genConstant(Val);
}

Value *VariableExprNode::codegen(CodeGenerator *CodeGen) {
  return CodeGen->genVariable(Name);
}

Value *BinaryExprNode::codegen(CodeGenerator *CodeGen) {
  Value *LHSVal = LHSNode->codegen(CodeGen);
  Value *RHSVal = RHSNode->codegen(CodeGen);
  return CodeGen->genBinOp(Operator, LHSVal, RHSVal);
}

Value *CallExprNode::codegen(CodeGenerator *CodeGen) {
  std::vector<Value *> FuncArgs;
  for (auto &&N : Args)
    FuncArgs.push_back(N->codegen(CodeGen));

  return CodeGen->genCall(Callee, FuncArgs);
}

Function *PrototypeNode::codegen(CodeGenerator *CodeGen) {
  return CodeGen->genPrototype(FuncName, ArgsNames);
}

Value *FunctionNode::codegen(CodeGenerator *CodeGen) {
  CodeGen->clearSymbolTable();
  Function *FuncProto = Prototype->codegen(CodeGen);
  CodeGen->createEntryBasicBlock(FuncProto);
  Value *FuncBody = Body->codegen(CodeGen);

  return CodeGen->genFunction(FuncProto, FuncBody);
}

