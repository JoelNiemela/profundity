#include "comptime.h"
#include "type.h"

ComptimeEngine::ComptimeEngine(Symtable* symtable) : symtable(new Symtable(symtable)) {}

Value* ComptimeEngine::eval_exp(Exp *exp) {
	VarExp* var_exp;
	if ((var_exp = dynamic_cast<VarExp*>(exp))) {
		std::optional<Symbol> symbol = symtable->lookup(var_exp->id);
		if (symbol) {
			return symbol->value;
		}
	}

	return nullptr;
}

Type* ComptimeEngine::eval_type_exp(Exp *exp) {
	Value* value = eval_exp(exp);

	Type* type;
	if ((type = dynamic_cast<Type*>(value))) {
		return type;
	}

	return nullptr;
}
