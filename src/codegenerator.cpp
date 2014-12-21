#include "codegenerator.h"
#include "node.h"

#include "llvm/ADT/APFloat.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Value.h"
#include "llvm/IR/Verifier.h"

#include "llvm/Support/raw_ostream.h"

#include <cassert>

using namespace javamm;
using namespace llvm;

//===----------------------------------------------------------------------===//
// Implementation of the core language code generation
//

void CodeGenerator::dumpIR() { TheModule->dump(); }

//===----------------------------------------------------------------------===//
// Implementation of the core language code generation
//

void CodeGenerator::genConstant(double Val) {
  CurrenVal = ConstantFP::get(TheBuilder->getContext(), APFloat(Val));
}

void CodeGenerator::genVariable(const std::string &Val) {
  CurrenVal = SymbolTable[Val];
}

void CodeGenerator::genBinOp(char Op, ExprNode *LHS, ExprNode *RHS) {
  assert(RHS != nullptr && "BinOp: rhs operand null");
  assert(LHS != nullptr && "BinOp: lhs operand null");

  LHS->codegen(this);
  if (CurrenVal == nullptr)
    return;
  Value *LHSVal = CurrenVal;

  RHS->codegen(this);
  if (CurrenVal == nullptr)
    return;
  Value *RHSVal = CurrenVal;

  switch (Op) {
  case '+':
    CurrenVal = TheBuilder->CreateFAdd(LHSVal, RHSVal);
    break;
  case '-':
    CurrenVal = TheBuilder->CreateFSub(LHSVal, RHSVal);
    break;
  case '*':
    CurrenVal = TheBuilder->CreateFMul(LHSVal, RHSVal);
    break;
  case '/':
    CurrenVal = TheBuilder->CreateFDiv(LHSVal, RHSVal);
    break;
  case '<':
    CurrenVal = TheBuilder->CreateFCmpULT(LHSVal, RHSVal);
    break;
  case '>':
    CurrenVal = TheBuilder->CreateFCmpUGT(LHSVal, RHSVal);
    break;
  default:
    CurrenVal = nullptr;
  }
}

void CodeGenerator::genCall(const std::string &FuncName,
                              const std::vector<ExprNode *> Args) {
  Function *F = TheModule->getFunction(FuncName);
  if (F == nullptr) {
    errs() << "Call: function doesn't exists\n";
    return;
  }

  if (F->arg_size() != Args.size()) {
    errs() << "Call: wrong number of arguments. Expected " << F->arg_size()
           << ", given " << Args.size() << "\n";
    ;
    return;
  }

  std::vector<Value *> ArgsVals(Args.size());
  for (auto &&EN : Args) {
    EN->codegen(this);
    ArgsVals.push_back(CurrenVal);
  }

  CurrenVal = TheBuilder->CreateCall(F, ArgsVals);
}

void CodeGenerator::genPrototype(const std::string &FuncName,
                                 const std::vector<std::string> Args) {
  FunctionType *FT = FunctionType::get(
      Type::getDoubleTy(TheBuilder->getContext()),
      std::vector<Type *>(Args.size(),
                          Type::getDoubleTy(TheBuilder->getContext())),
      false);
  Function *F = Function::Create(FT, Function::ExternalLinkage, FuncName,
                                 TheModule.get());

  // TODO: Don't allow redefinition

  unsigned Index = 0;
  for (auto ArgIt = F->arg_begin(), ArgEnd = F->arg_end(); ArgIt != ArgEnd;
       ++ArgIt) {
    ArgIt->setName(Args[Index]);
    SymbolTable[Args[Index++]] = ArgIt;
  }

  CurrenVal = F;
}

void CodeGenerator::genFunction(PrototypeNode *Prototype, ExprNode *Body) {
  assert(Prototype != nullptr && "Function: prototype null");
  assert(Body != nullptr && "Function: body null");

  Prototype->codegen(this);
  if (CurrenVal == nullptr)
    return;

  Function *F = cast<Function>(CurrenVal);

  BasicBlock *BB = BasicBlock::Create(TheBuilder->getContext(), "entry", F);
  TheBuilder->SetInsertPoint(BB);

  Body->codegen(this);
  if (CurrenVal != nullptr) {
    outs() << "NIGGA\n";
    TheBuilder->CreateRet(CurrenVal);

    verifyFunction(*F);
    CurrenVal = F;
  } else {
    F->eraseFromParent();
  }
}

//===----------------------------------------------------------------------===//
// Implementation of the control flow code generation
//

void CodeGenerator::genIf(ExprNode *Cond, ExprNode *Then, ExprNode *Else) {
  assert(Cond != nullptr && "If: condition is null");
  assert(Then != nullptr && "If: then block is null");
  assert(Else != nullptr && "If: else block is null");

  Cond->codegen(this);
  if(CurrenVal == nullptr)
      return;
  Value *CondVal = CurrenVal;

  CondVal = TheBuilder->CreateFCmpONE(
      CondVal, ConstantFP::get(TheBuilder->getContext(), APFloat(0.0)));

  Function *F = TheBuilder->GetInsertBlock()->getParent();

  BasicBlock *ThenBB = BasicBlock::Create(TheBuilder->getContext(), "", F);
  BasicBlock *ElseBB = BasicBlock::Create(TheBuilder->getContext());
  BasicBlock *MergeBB = BasicBlock::Create(TheBuilder->getContext());

  TheBuilder->CreateCondBr(CondVal, ThenBB, ElseBB);

  TheBuilder->SetInsertPoint(ThenBB);

  Then->codegen(this);
  if(CurrenVal == nullptr)
      return;
  Value *ThenVal = CurrenVal;

  TheBuilder->CreateBr(MergeBB);
  ThenBB = TheBuilder->GetInsertBlock();

  F->getBasicBlockList().push_back(ElseBB);
  TheBuilder->SetInsertPoint(ElseBB);

  Else->codegen(this);
  if(CurrenVal == nullptr)
      return;
  Value *ElseVal = CurrenVal;

  TheBuilder->CreateBr(MergeBB);
  ElseBB = TheBuilder->GetInsertBlock();

  F->getBasicBlockList().push_back(MergeBB);
  TheBuilder->SetInsertPoint(MergeBB);
  PHINode *PN =
      TheBuilder->CreatePHI(Type::getDoubleTy(TheBuilder->getContext()), 2);

  PN->addIncoming(ThenVal, ThenBB);
  PN->addIncoming(ElseVal, ElseBB);
}

//Value *CodeGenerator::genFor() {
//  return nullptr;
//}
//
//Value *CodeGenerator::genWhile() {
//  return nullptr;
//}

