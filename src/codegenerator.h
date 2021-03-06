//===---------- codegenerator.h - Code generator class definition ---------===//
/// \file
///
/// This file contains the logic to generate LLVM IR from an AST.
///
//===----------------------------------------------------------------------===//

#ifndef JAVAMM_CODE_GENERATOR_H
#define JAVAMM_CODE_GENERATOR_H

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/raw_ostream.h"

#include <memory>
#include <string>
#include <unordered_map>

namespace llvm {
  class AllocaInst;
  class Function;
}

namespace javamm {

class ExprNode;
class PrototypeNode;

/// \brief CodeGenerator
/// This class is a visitor that recursively traverse the AST to generate LLVM
/// IR. This process is known a syntax-directed translation.
class CodeGenerator {
public:
  CodeGenerator()
      : TheModule{std::make_unique<llvm::Module>("javamm Module",
                                                 llvm::getGlobalContext())},
        TheBuilder{
            std::make_unique<llvm::IRBuilder<>>(TheModule->getContext())}, CurrenVal{nullptr} {}

public: // Utilities function 
  void dumpIR() const;
  // void dumpAsm();
  void printIR(llvm::raw_ostream &OS) const;

public: // Core language code generation
  void genConstant(double Val);
  void genDecl(const std::string &Var, ExprNode *Body);
  void genVariable(const std::string &Val);
  void genBinOp(char Op, ExprNode *LHS, ExprNode *RHS);
  void genCall(const std::string &FuncName, const std::vector<ExprNode *> Args);
  void genPrototype(const std::string &FuncName,
                    const std::vector<std::string> Args);
  void genFunction(PrototypeNode *Prototype, ExprNode *Body);

public: // Control flow code generation
  void genIf(ExprNode *Cond, ExprNode *Then, ExprNode *Else);
  void genFor(const std::string &VarName, ExprNode *Begin, ExprNode *End,
              ExprNode *Step, ExprNode *Body);
  void genWhile(ExprNode *Cond, ExprNode *Body);

public: // IO code generation
  void genIO(const std::string &Message);

private:
  llvm::AllocaInst *createEntryBlockAlloca(llvm::Function *F,
                                           const std::string &VarName);
  void createArgumentAllocas(llvm::Function *F, const std::vector<std::string> &VarNames);

private:
  std::unordered_map<std::string, llvm::AllocaInst*> SymbolTable;
  std::unique_ptr<llvm::Module> TheModule;
  std::unique_ptr<llvm::IRBuilder<>> TheBuilder;
  llvm::Value *CurrenVal;
};

} // End namespace javamm

#endif // JAVAMM_CODE_GENERATOR_H

