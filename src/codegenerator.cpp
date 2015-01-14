#include "codegenerator.h"
#include "node.h"

#include "llvm/ADT/APFloat.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Value.h"
#include "llvm/IR/Verifier.h"

#include "llvm/Support/raw_ostream.h"

#include <cassert>

using namespace javamm;
using namespace llvm;

//===----------------------------------------------------------------------===//
// Implementation of the utilities functions
//

void CodeGenerator::dumpIR() const { TheModule->dump(); }

void CodeGenerator::printIR(raw_ostream &OS) const {
  TheModule->print(OS, nullptr);
}

AllocaInst *CodeGenerator::createEntryBlockAlloca(Function *F,
                                                  const std::string &VarName) {
  TheBuilder->SetInsertPoint(&F->getEntryBlock());
  AllocaInst *I =
      TheBuilder->CreateAlloca(TheBuilder->getDoubleTy(), nullptr, VarName);
  return I;
}

void CodeGenerator::createArgumentAllocas(
    Function *F, const std::vector<std::string> &VarNames) {
  auto ArgIt = F->begin();
  for (unsigned i = 0, e = F->arg_size(); i != e; ++i, ++ArgIt) {
    AllocaInst *AllocaVal = createEntryBlockAlloca(F, VarNames[i]);
    TheBuilder->CreateStore(ArgIt, AllocaVal);
    SymbolTable[VarNames[i]] = AllocaVal;
  }
}

//===----------------------------------------------------------------------===//
// Implementation of the core language code generation
//

void CodeGenerator::genConstant(double Val) {
  CurrenVal = ConstantFP::get(TheBuilder->getContext(), APFloat(Val));
}

void CodeGenerator::genDecl(const std::string &Var,
                            ExprNode *Body) {
  assert(Body != nullptr && "Decl: body is null");

  Function *F = TheBuilder->GetInsertBlock()->getParent();

  Body->codegen(this);
  if (CurrenVal == nullptr)
    return;
  Value *BodyVal = CurrenVal;

  AllocaInst *AllocaVal = createEntryBlockAlloca(F, Var);
  TheBuilder->CreateStore(BodyVal, AllocaVal);
}

void CodeGenerator::genVariable(const std::string &Val) {
  Value *V = SymbolTable[Val];
  // TODO: What if V == nullptr?
  CurrenVal = TheBuilder->CreateLoad(V, Val);
}

void CodeGenerator::genBinOp(char Op, ExprNode *LHS, ExprNode *RHS) {
  assert(RHS != nullptr && "BinOp: rhs operand null");
  assert(LHS != nullptr && "BinOp: lhs operand null");

  // An assignment is a special case where we don't want to generate the LHS
  if (Op == '=') {
    VariableExprNode *VarNode = dyn_cast<VariableExprNode>(LHS);
    if (VarNode == nullptr)
      return;

    RHS->codegen(this);
    if (CurrenVal == nullptr)
      return;
    Value *RHSVal = CurrenVal;

    Value *Var = SymbolTable[VarNode->getName()];
    if (Var == nullptr)
      return;

    TheBuilder->CreateStore(RHSVal, Var);
    CurrenVal = RHSVal;
  }

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
      TheBuilder->getDoubleTy(),
      std::vector<Type *>(Args.size(), TheBuilder->getDoubleTy()), false);
  Function *F = Function::Create(FT, Function::ExternalLinkage, FuncName,
                                 TheModule.get());

  // TODO: Don't allow redefinition

  unsigned Index = 0;
  for (auto ArgIt = F->arg_begin(), ArgEnd = F->arg_end(); ArgIt != ArgEnd;
       ++ArgIt) {
    ArgIt->setName(Args[Index]);
  }

  CurrenVal = F;
}

void CodeGenerator::genFunction(PrototypeNode *Prototype, ExprNode *Body) {
  assert(Prototype != nullptr && "Function: prototype null");
  assert(Body != nullptr && "Function: body null");

  SymbolTable.clear();

  Prototype->codegen(this);
  if (CurrenVal == nullptr)
    return;

  Function *F = cast<Function>(CurrenVal);

  BasicBlock *BB = BasicBlock::Create(TheBuilder->getContext(), "entry", F);
  TheBuilder->SetInsertPoint(BB);

  createArgumentAllocas(F, Prototype->getArgsNames());

  Body->codegen(this);
  if (CurrenVal != nullptr) {
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
  PHINode *PN = TheBuilder->CreatePHI(TheBuilder->getDoubleTy(), 2);

  PN->addIncoming(ThenVal, ThenBB);
  PN->addIncoming(ElseVal, ElseBB);
}

void CodeGenerator::genFor(const std::string &VarName, ExprNode *Begin,
                           ExprNode *End, ExprNode *Step, ExprNode *Body) {
  assert(Begin != nullptr && "For: begin block is null");
  assert(End != nullptr && "For: end block is null");
  assert(Step != nullptr && "For: step block is null");

  Function *F = TheBuilder->GetInsertBlock()->getParent();
  AllocaInst *AllocaVal = createEntryBlockAlloca(F, VarName);

  Begin->codegen(this);
  if (CurrenVal == nullptr)
    return;
  Value *BeginVal = CurrenVal;

  TheBuilder->CreateStore(BeginVal, AllocaVal);

  BasicBlock *HeaderBB = BasicBlock::Create(TheBuilder->getContext(), "", F);

  TheBuilder->CreateBr(HeaderBB);

  TheBuilder->SetInsertPoint(HeaderBB);

  AllocaInst *OldVal = SymbolTable[VarName];
  SymbolTable[VarName] = AllocaVal;

  Body->codegen(this);
  if (CurrenVal == nullptr)
    return;

  Step->codegen(this);
  if (CurrenVal == nullptr)
    return;
  Value *StepVal = CurrenVal;

  End->codegen(this);
  if (CurrenVal == nullptr)
    return;
  Value *EndVal = CurrenVal;

  Value *CurVar = TheBuilder->CreateLoad(AllocaVal, VarName);
  Value *NextVar = TheBuilder->CreateFAdd(CurVar, StepVal);
  TheBuilder->CreateStore(NextVar, AllocaVal);

  //EndVal = TheBuilder->CreateFCmpONE(
  //    EndVal, ConstantFP::get(TheBuilder->getContext(), APFloat(0.0)));

  BasicBlock *AfterBB = BasicBlock::Create(TheBuilder->getContext(), "", F);

  TheBuilder->CreateCondBr(EndVal, HeaderBB, AfterBB);

  TheBuilder->SetInsertPoint(AfterBB);

  if (OldVal != nullptr)
    SymbolTable[VarName] = OldVal;
  else
    SymbolTable.erase(VarName);

  CurrenVal = Constant::getNullValue(TheBuilder->getDoubleTy());
}

void CodeGenerator::genWhile(ExprNode *Cond, ExprNode *Body) {
  assert(Cond != nullptr && "While: condition is null");

  Function *F = TheBuilder->GetInsertBlock()->getParent();

  BasicBlock *HeaderBB = BasicBlock::Create(TheBuilder->getContext(), "", F);

  TheBuilder->CreateBr(HeaderBB);

  TheBuilder->SetInsertPoint(HeaderBB);

  Cond->codegen(this);
  if (CurrenVal == nullptr)
    return;
  Value *CondVal = CurrenVal;

  CondVal = TheBuilder->CreateFCmpONE(
      CondVal, ConstantFP::get(TheBuilder->getContext(), APFloat(0.0)));

  Body->codegen(this);
  if (CurrenVal == nullptr)
    return;

  BasicBlock *ContBB = BasicBlock::Create(TheBuilder->getContext());

  TheBuilder->CreateCondBr(CondVal, HeaderBB, ContBB);

  TheBuilder->SetInsertPoint(ContBB);
}

//===----------------------------------------------------------------------===//
// Implementation of the IO code generation
//

void CodeGenerator::genIO(const std::string &Message) {
  Function *PrintF =
      TheBuilder->GetInsertBlock()->getParent()->getParent()->getFunction(
          "println");
  if (PrintF == nullptr) {
    FunctionType *FT = FunctionType::get(
        TheBuilder->getVoidTy(), Type::getInt8PtrTy(TheBuilder->getContext()));
    PrintF = Function::Create(FT, Function::ExternalLinkage, "println",
                              TheModule.get());
  }

  // TODO: Will this create duplicates?
  Value *MessageInst = TheBuilder->CreateGlobalStringPtr(Message);

  TheBuilder->CreateCall(PrintF, MessageInst);
}

