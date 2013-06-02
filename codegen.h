#ifndef CODEGEN_H
#define CODEGEN_H

#include <stack>

#include <llvm/CallingConv.h>
#include <llvm/DerivedTypes>
#include <llvm/Function.h>
#include <llvm/Instructions.h>
#include <llvm/LLVMContext.h>
#include <llvm/Module.h>
#include <llvm/PassManager.h>
#include <llvm/Type.h>

#include <llvm/Analysis/Verifier.h>

#include <llvm/Assembly/PrintModulePass.h>

#include <llvm/Bitcode/ReaderWriter.h>

#include <llvm/Support/

class CodeGen
{
public:
	CodeGen();
	~CodeGen();
};

#endif // CODEGEN_H
