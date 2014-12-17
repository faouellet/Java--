#ifndef JAVAMM_CODE_GENERATOR_H
#define JAVAMM_CODE_GENERATOR_H

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"

#include <memory>
#include <string>
#include <unordered_map>

namespace llvm {
  class Function;
  class Value;
}

namespace javamm {

/// CodeGenerator - TODO
class CodeGenerator {
public:
  CodeGenerator()
      : TheModule{std::make_unique<llvm::Module>("javamm Module",
                                                 llvm::getGlobalContext())},
        TheBuilder{
            std::make_unique<llvm::IRBuilder<>>(TheModule->getContext())} {}

public: // Output functions
  void dumpIR();
  // void dumpAsm();

public: // Core language generation
  llvm::Value *genConstant(double Val);
  llvm::Value *genVariable(const std::string &Val);
  llvm::Value *genBinOp(char Op, llvm::Value *LHS, llvm::Value *RHS);
  llvm::Value *genCall(const std::string &FuncName,
                       const std::vector<llvm::Value *> Args);
  llvm::Function *genPrototype(const std::string &FuncName,
                            const std::vector<std::string> Args);
  llvm::Value *genFunction(llvm::Function *F, llvm::Value *Body);

//public: // Control flow generation
//  llvm::Value *genIf();
//  llvm::Value *genFor();
//  llvm::Value *genWhile();

private:
  std::unordered_map<std::string, llvm::Value *> SymbolTable;
  std::unique_ptr<llvm::Module> TheModule;
  std::unique_ptr<llvm::IRBuilder<>> TheBuilder;
};

} // End namespace javamm

#endif // JAVAMM_CODE_GENERATOR_H

