#include "comptime.h"
#include "ast.h"
#include "type.h"

ComptimeState::ComptimeState() {}

Value* ComptimeState::run_exp(Exp *exp) {
	if (ValueExp* value_exp; (value_exp = dynamic_cast<ValueExp*>(exp))) {
		return value_exp->value;
	} else if (UnitExp* unit_exp; (unit_exp = dynamic_cast<UnitExp*>(exp))) {
		return new TypeValue(new ComptimeType(ComptimeType::UNIT));
	} else if (OpExp* op_exp; (op_exp = dynamic_cast<OpExp*>(exp))) {
		switch (op_exp->op.type) {
			case Op::ARROW: {
				TypeValue* lval = dynamic_cast<TypeValue*>(this->run_exp(op_exp->lexp));
				TypeValue* rval = dynamic_cast<TypeValue*>(this->run_exp(op_exp->rexp));
				if (lval && rval) {
					return new TypeValue(new FunctionType(lval->value_type, rval->value_type));
				} else {
					return nullptr;
				}
			} break;
			default:
				return nullptr;
		}
	}

	return nullptr;
}
