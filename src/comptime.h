#ifndef COMPTIME_H
#define COMPTIME_H

#include "ast.h"
#include "symtable.h"
#include "type.h"

class ComptimeEngine {
private:
	Symtable* symtable;
public:
	ComptimeEngine(Symtable* symtable);

	Value* eval_exp(Exp* exp);
	Type* eval_type_exp(Exp* exp);
};

#endif
