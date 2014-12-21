#ifndef JAVAMM_CODE_GENERATOR_H
#define JAVAMM_CODE_GENERATOR_H

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"

#include <memory>
#include <string>
#include <unordered_map>

namespace llvm {
  class Value;
}

namespace javamm {

class ExprNode;
class PrototypeNode;

/// CodeGenerator - TODO
class CodeGenerator {
public:
  CodeGenerator()
      : TheModule{std::make_unique<llvm::Module>("javamm Module",
                                                 llvm::getGlobalContext())},
        TheBuilder{
            std::make_unique<llvm::IRBuilder<>>(TheModule->getContext())}, CurrenVal{nullptr} {}

public: // Utilities function 
  void dumpIR();
  // void dumpAsm();

public: // Core language generation
  void genConstant(double Val);
  void genVariable(const std::string &Val);
  void genBinOp(char Op, ExprNode *LHS, ExprNode *RHS);
  void genCall(const std::string &FuncName, const std::vector<ExprNode *> Args);
  void genPrototype(const std::string &FuncName,
                    const std::vector<std::string> Args);
  void genFunction(PrototypeNode *Prototype, ExprNode *Body);

public: // Control flow generation
  void genIf(ExprNode *Cond, ExprNode *Then, ExprNode *Else);
//  llvm::Value *genFor();
//  llvm::Value *genWhile();

private:
  std::unordered_map<std::string, llvm::Value *> SymbolTable;
  std::unique_ptr<llvm::Module> TheModule;
  std::unique_ptr<llvm::IRBuilder<>> TheBuilder;
  llvm::Value *CurrenVal;
};

} // End namespace javamm

#endif // JAVAMM_CODE_GENERATOR_H

