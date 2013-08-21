#include "types.h"

////////// Type //////////
Type::Type() { }

Type::~Type() { }

////////// ArrayType //////////
ArrayType::ArrayType() { }

ArrayType::~ArrayType() { }

void ArrayType::Accept(std::unique_ptr<CodeGen> in_CG) { }
