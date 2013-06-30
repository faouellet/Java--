#ifndef CODEGEN_H
#define CODEGEN_H

#include <stack>

#include <llvm/PassManager.h>

#include <llvm/Analysis/Verifier.h>

#include <llvm/Assembly/PrintModulePass.h>

#include <llvm/Bitcode/ReaderWriter.h>

#include <llvm/ExecutionEngine/GenericValue.h>
#include <llvm/ExecutionEngine/JIT.h>

#include <llvm/IR/CallingConv.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Type.h>

#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/TargetSelect.h>

//#include <llvm/ModuleProvider.h>

using namespace llvm;

class NBlock;

struct CodeGenBlock
{
	BasicBlock* Block;
	std::map<std::string, Value*> Locals;
};

class CodeGenContext
{
private:
	std::stack<CodeGenBlock*> m_Blocks;
	Function* m_MainFunction;

public:
	// TODO : Ewww...
	Module* Mod;

public:
	CodeGenContext() { Mod = new Module("main", getGlobalContext()); }
	~CodeGenContext() { delete Mod; }

	void GenerateCode(NBlock & in_Root);
	GenericValue RunCode();
	std::map<std::string, Value*>& Locals() { return m_Blocks.top()->Locals; } 
	BasicBlock* CurrentBlock() { return m_Blocks.top()->Block; }
	void PushBlock(BasicBlock * in_Block) { m_Blocks.push(new CodeGenBlock()); m_Blocks.top()->Block = in_Block; }
	void PopBlock() { CodeGenBlock* l_Top = m_Blocks.top(); m_Blocks.pop(); delete l_Top; }
};

#endif // CODEGEN_H
