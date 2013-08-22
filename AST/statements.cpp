#include "statements.h"

////////// Statement //////////
Statement::Statement() { }

Statement::~Statement() { }


////////// Assignment //////////
Assignment::Assignment() { }

Assignment::~Assignment() { }

void Assignment::Accept(std::unique_ptr<CodeGen> in_CG)
{
	in_CG->Visit(this);
}


////////// IfStatement //////////
IfStatement::IfStatement() { }

IfStatement::~IfStatement() { }

void IfStatement::Accept(std::unique_ptr<CodeGen> in_CG)
{
	in_CG->Visit(this);
}


////////// ProcedureCall //////////
ProcedureCall::ProcedureCall() { }

ProcedureCall::~ProcedureCall() { }

void ProcedureCall::Accept(std::unique_ptr<CodeGen> in_CG)
{
	in_CG->Visit(this);
}


////////// ReturnStatement //////////
ReturnStatement::ReturnStatement() { }

ReturnStatement::~ReturnStatement() { }

void ReturnStatement::Accept(std::unique_ptr<CodeGen> in_CG)
{
	in_CG->Visit(this);
}


////////// WhileStatement //////////
WhileStatement::WhileStatement() { }

WhileStatement::~WhileStatement() { }

void WhileStatement::Accept(std::unique_ptr<CodeGen> in_CG)
{
	in_CG->Visit(this);
}