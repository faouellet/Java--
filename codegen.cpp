#include "codegen.h"

#include "codegen.h"
#include "parser.hpp"

void CodeGenContext::GenerateBlock(NBlock & in_Block)
{
	std::cout << "Genrating code..." << std::endl;

	std::vector<const Type*> l_ArgTypes;
	FunctionType* l_FType = FunctionType::get(Type::getVoidTy(getGlobalContext()), l_ArgTypes, false);
	m_MainFunction = Function::Create(l_FType, GlobalValue::InternalLinkage)
}


