#include "statements.h"

////////// Statement //////////
Statement::Statement() { }

Statement::~Statement() { }

////////// ReturnStatement //////////
ReturnStatement::ReturnStatement() { }

ReturnStatement::~ReturnStatement() { }

void ReturnStatement::Accept(std::unique_ptr<CodeGen> in_CG)
{
	in_CG->Visit(this);
}
