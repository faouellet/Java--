#include "codegenerator.h"

#include "llvm/ADT/APFloat.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Value.h"

using namespace javamm;
using namespace llvm;

//===----------------------------------------------------------------------===//
// Implementation of the output functions
//

void CodeGenerator::dumpIR() { TheModule->dump(); }

//===----------------------------------------------------------------------===//
// Implementation of the core language code generation
//

Value *CodeGenerator::genConstant(double Val) {
  return ConstantFP::get(TheBuilder->getContext(), APFloat(Val));
}

Value *CodeGenerator::genVariable(const std::string &Val) {
  return SymbolTable[Val];
}

Value *CodeGenerator::genBinOp(char Op, Value *LHS, Value *RHS) {
  if (LHS == nullptr || RHS == nullptr)
    return nullptr;

  switch (Op) {
  case '+':
    return TheBuilder->CreateFAdd(LHS, RHS);
  case '-':
    return TheBuilder->CreateFSub(LHS, RHS);
  case '*':
    return TheBuilder->CreateFMul(LHS, RHS);
  case '/':
    return TheBuilder->CreateFDiv(LHS, RHS);
  case '<':
    return TheBuilder->CreateFCmpULT(LHS, RHS);
  case '>':
    return TheBuilder->CreateFCmpUGT(LHS, RHS);
  default:
    return nullptr;
  }
}

Value *CodeGenerator::genCall(const std::string &FuncName,
                              const std::vector<Value *> Args) {
  Function *F = TheModule->getFunction(FuncName);
  if (F == nullptr)
    return nullptr;

  if (F->arg_size() != Args.size())
    return nullptr;

  return TheBuilder->CreateCall(F, Args);
}

Function *CodeGenerator::genPrototype(const std::string &FuncName,
                                   const std::vector<std::string> Args) {
  FunctionType *FT = FunctionType::get(
      Type::getDoubleTy(TheBuilder->getContext()),
      std::vector<Type *>(Args.size(),
                          Type::getDoubleTy(TheBuilder->getContext())),
      false);
  Function *F = Function::Create(FT, Function::ExternalLinkage, FuncName,
                                 TheModule.get());

  unsigned Index = 0;
  for (auto ArgIt = F->arg_begin(), ArgEnd = F->arg_end(); ArgIt != ArgEnd;
       ++ArgIt) {
    ArgIt->setName(Args[Index]);
    SymbolTable[Args[Index++]] = ArgIt;
  }

  return F;
}

Value *CodeGenerator::genFunction(Function *F, Value *Body) {
  if (F == nullptr || Body == nullptr)
    return nullptr;

  SymbolTable.clear();

  BasicBlock *BB = BasicBlock::Create(TheBuilder->getContext(), "entry", F);
  TheBuilder->SetInsertPoint(BB);
  TheBuilder->CreateRet(Body);

  return F;
}

//===----------------------------------------------------------------------===//
// Implementation of the control flow code generation
//

//Value *CodeGenerator::genIf() {
//  return nullptr;
//}
//
//Value *CodeGenerator::genFor() {
//  return nullptr;
//}
//
//Value *CodeGenerator::genWhile() {
//  return nullptr;
//}

