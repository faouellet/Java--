#include "codegen.h"
#include "node.h"
#include "parser.hpp"

void CodeGenContext::GenerateCode(NBlock & in_Root)
{
	std::cout << "Genrating code..." << std::endl;

	FunctionType* l_FType = FunctionType::get(Type::getVoidTy(getGlobalContext()), std::vector<Type*>(), false);
	m_MainFunction = Function::Create(l_FType, GlobalValue::InternalLinkage, "main", Mod);
	BasicBlock* l_Block = BasicBlock::Create(getGlobalContext(), "entry", m_MainFunction);

	PushBlock(l_Block);
	in_Root.CodeGen(*this);
	ReturnInst::Create(getGlobalContext(), l_Block);
	PopBlock();

	std::cout << "Done" << std::endl;
	PassManager l_PassMgr;
	l_PassMgr.add(createPrintModulePass(&outs()));
	l_PassMgr.run(*Mod);
}

GenericValue CodeGenContext::RunCode()
{
	std::cout << "Running code..." << std::endl;
	ExecutionEngine* l_EEngine = ExecutionEngine::create(Mod);
	std::vector<GenericValue> l_NoArgs;
	GenericValue l_GenValue = l_EEngine->runFunction(m_MainFunction, l_NoArgs);
	std::cout << "Code was run" << std::endl;
	return l_GenValue;
}

static Type* TypeOf(const NIdentifier & in_Type)
{
	if(in_Type.Name.compare("int") == 0)
	{
		return Type::getInt64Ty(getGlobalContext());
	}
	else if(in_Type.Name.compare("double") == 0)
	{
		return Type::getDoubleTy(getGlobalContext());
	}
	return Type::getVoidTy(getGlobalContext());
}

/////////// Code Generation /////////// 

Value* NInteger::CodeGen(CodeGenContext & io_Context)
{
	std::cout << "Creating integer" << std::endl;
	return ConstantInt::get(Type::getInt64Ty(getGlobalContext()), Value, true);
}

Value* NDouble::CodeGen(CodeGenContext & io_Context)
{
	std::cout << "Creating double" << std::endl;
	return ConstantFP::get(Type::getDoubleTy(getGlobalContext()), Value);
}

Value* NIdentifier::CodeGen(CodeGenContext & io_Context)
{
	std::cout << "Creating identifier reference: " << Name << std::endl;
	if(io_Context.Locals().find(Name) == io_Context.Locals().end())
	{
		std::cerr << "undeclared variable " << Name << std::endl;
		return nullptr;
	}
	return new LoadInst(io_Context.Locals()[Name], "", false, io_Context.CurrentBlock());
}

Value* NMethodCall::CodeGen(CodeGenContext & io_Context)
{
	Function* l_Function = io_Context.Mod->getFunction(ID.Name.c_str());
	if(l_Function == nullptr)
	{
		std::cerr << "no such function " << ID.Name << std::endl;
	}

	std::vector<Value*> l_Args;
	for(auto& in_Arg : Arguments)
	{
		l_Args.push_back(in_Arg->CodeGen(io_Context));
	}
	CallInst* l_Call = CallInst::Create(l_Function, l_Args, "", io_Context.CurrentBlock());
	std::cout << "Creating method call: " << ID.Name << std::endl;
	return l_Call;
}

Value* NBinaryOperator::CodeGen(CodeGenContext & io_Context)
{
	std::cout << "Creating binary operation " << Op << std::endl;
	Instruction::BinaryOps l_Instr;
	switch (Op) 
	{
		case TPLUS: 	l_Instr = Instruction::Add; goto math;
		case TMINUS: 	l_Instr = Instruction::Sub; goto math;
		case TMUL: 		l_Instr = Instruction::Mul; goto math;
		case TDIV: 		l_Instr = Instruction::SDiv; goto math;
				
		/* TODO comparison */
	}

	return NULL;
math:
	return BinaryOperator::Create(l_Instr, LHS.CodeGen(io_Context), 
		RHS.CodeGen(io_Context), "", io_Context.CurrentBlock());
}

Value* NAssignment::CodeGen(CodeGenContext & io_Context)
{
	std::cout << "Creating assignment for " << LHS.Name << std::endl;
	if (io_Context.Locals().find(LHS.Name) == io_Context.Locals().end())
	{
		std::cerr << "undeclared variable " << LHS.Name << std::endl;
		return nullptr;
	}
	return new StoreInst(RHS.CodeGen(io_Context), io_Context.Locals()[LHS.Name], false, io_Context.CurrentBlock());
}

Value* NBlock::CodeGen(CodeGenContext & io_Context)
{
	Value* l_Last = nullptr;

	for(auto& in_Stmt : Statements)
	{
		std::cout << "Generating code for " << typeid(in_Stmt).name() << std::endl;
		l_Last = in_Stmt->CodeGen(io_Context);
	}

	std::cout << "Creating block" << std::endl;
	return l_Last;
}

Value* NExpressionStatement::CodeGen(CodeGenContext & io_Context)
{
	std::cout << "Generating code for " << typeid(Expression).name() << std::endl;
	return Expression.CodeGen(io_Context);
}

Value* NVariableDeclaration::CodeGen(CodeGenContext & io_Context)
{
	std::cout << "Creating variable declaration " << Type.Name << " " << ID.Name << std::endl;
	AllocaInst* l_Alloc = new AllocaInst(TypeOf(Type), ID.Name.c_str(), io_Context.CurrentBlock());
	io_Context.Locals()[ID.Name] = l_Alloc;
	if (AssignmentExpr != nullptr)
	{
		NAssignment l_Assn(ID, *AssignmentExpr);
		l_Assn.CodeGen(io_Context);
	}
	return l_Alloc;
}

Value* NFunctionDeclaration::CodeGen(CodeGenContext & io_Context)
{
	std::vector<llvm::Type*> l_ArgTypes;

	for(auto& in_Arg : Arguments)
	{
		l_ArgTypes.push_back(TypeOf((*in_Arg).Type));
	}

	FunctionType* l_FType = FunctionType::get(TypeOf(Type), l_ArgTypes, false);
	Function* l_Function = Function::Create(l_FType, GlobalValue::InternalLinkage, ID.Name.c_str(), io_Context.Mod);
	BasicBlock* l_Block = BasicBlock::Create(getGlobalContext(), "entry", l_Function, 0);

	io_Context.PushBlock(l_Block);

	for(auto& in_Arg : Arguments)
	{
		in_Arg->CodeGen(io_Context);
	}
	
	Block.CodeGen(io_Context);
	ReturnInst::Create(getGlobalContext(), l_Block);

	io_Context.PopBlock();
	std::cout << "Creating function: " << ID.Name << std::endl;
	return l_Function;
}
