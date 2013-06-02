#ifndef CODEGEN_H
#define CODEGEN_H

#include <stack>

#include <llvm/CallingConv.h>
#include <llvm/DerivedTypes>
#include <llvm/Function.h>
#include <llvm/Instructions.h>
#include <llvm/LLVMContext.h>
#include <llvm/Module.h>
#include <llvm/ModuleProvider.h>
#include <llvm/PassManager.h>
#include <llvm/Type.h>

#include <llvm/Analysis/Verifier.h>

#include <llvm/Assembly/PrintModulePass.h>

#include <llvm/Bitcode/ReaderWriter.h>

#include <llvm/ExecutionEngine/GenericValue.h>
#include <llvm/ExecutionEngine/JIT.h>

#include <llvm/Support/IRBuilder.h>
#include <llvm/Support/raw_ostream.h>

#include <llvm/Target/TargetSelect.h>

struct CodeGenBlock
{
	BasicBlock* Block;
	std::map<Std::string, Value*> Locals;
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
	CodeGenContext() { Mod = new Module(:"main", getGlobalContext()); }
	~CodeGenContext();

	void GenerateCode(NBlock & in_Root);
	GenericValue RunCode();
	std::map<std::string, Value*>& Locals() { return m_Blocks.top()->Locals; } 
	BasicBlock* CurrentBlock() { return m_Blocks.top()->Block; }
	void PushBlock(BasicBlock * in_Block) { m_Blocks.push(new CodeGenBlock()); m_Blocks.top()->Block = in_Block; }
	void PopBlock() { CodeGenBlock* l_Top = m_Blocks.top(); m_Blocks.pop(); delete l_Top; }
};

#endif // CODEGEN_H
