#include "declarations.h"


////////// Declaration //////////
Declaration::Declaration() { /*TODO: Something?*/ }

Declaration::~Declaration() { /*TODO: Something?*/ }

////////// FunctionDeclaration //////////
FunctionDeclaration::FunctionDeclaration() { }

FunctionDeclaration::~FunctionDeclaration() { }

void FunctionDeclaration::Accept(std::unique_ptr<CodeGen> in_CG)
{
	in_CG->Visit(this);
}

////////// VaraiableDeclaration //////////
VariableDeclaration::VariableDeclaration() { }

VariableDeclaration::~VariableDeclaration() { }

void VariableDeclaration::Accept(std::unique_ptr<CodeGen> in_CG)
{
	in_CG->Visit(this);
}
