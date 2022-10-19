#include "ast.h"

void Program::print(std::ostream& out, int indent) const {
	indented(out, indent) << "Program {\n";
	for (const Stm* stm : this->stms) {
		if (stm) stm->print(out, indent+1);
		else indented(out, indent+1) << "NULL\n";
	}
	indented(out, indent) << "}\n";
}

void ValueExp::print(std::ostream& out, int indent) const {
	indented(out, indent) << "ValueExp {\n";
	if (this->value) this->value->print(out, indent+1);
	else indented(out, indent+1) << "NULL\n";
	indented(out, indent) << "}\n";
}

void LetStm::print(std::ostream& out, int indent) const {
	indented(out, indent) << "LetStm {\n";
	indented(out, indent+1) << this->id << "\n";
	if (this->type) this->type->print(out, indent+1);
	else indented(out, indent+1) << "NULL\n";
	if (this->exp) this->exp->print(out, indent+1);
	else indented(out, indent+1) << "NULL\n";
	indented(out, indent) << "}\n";
}

void ExpStm::print(std::ostream& out, int indent) const {
	indented(out, indent) << "ExpStm {\n";
	if (this->exp) this->exp->print(out, indent+1);
	else indented(out, indent+1) << "NULL\n";
	indented(out, indent) << "}\n";
}

void UnitExp::print(std::ostream& out, int indent) const {
	indented(out, indent) << "UnitExp {\n";
	indented(out, indent) << "}\n";
}

void NumExp::print(std::ostream& out, int indent) const {
	indented(out, indent) << "NumExp {\n";
	indented(out, indent+1) << this->num << "\n";
	indented(out, indent) << "}\n";
}

void StrExp::print(std::ostream& out, int indent) const {
	indented(out, indent) << "StrExp {\n";
	indented(out, indent+1) << this->str << "\n";
	indented(out, indent) << "}\n";
}

void VarExp::print(std::ostream& out, int indent) const {
	indented(out, indent) << "VarExp {\n";
	indented(out, indent+1) << this->id << "\n";
	indented(out, indent) << "}\n";
}

void RecordExp::print(std::ostream& out, int indent) const {
	indented(out, indent) << "RecordExp {\n";
	indented(out, indent+1) << this->id << "\n";
	if (this->type_exp) this->type_exp->print(out, indent+1);
	else indented(out, indent+1) << "NULL\n";
	indented(out, indent) << "}\n";
}

void OpExp::print(std::ostream& out, int indent) const {
	indented(out, indent) << "OpExp {\n";
	if (this->lexp) this->lexp->print(out, indent+1);
	else indented(out, indent+1) << "NULL\n";
	if (this->rexp) this->rexp->print(out, indent+1);
	else indented(out, indent+1) << "NULL\n";
	indented(out, indent+1) << to_string(this->op.type) << "\n";
	indented(out, indent) << "}\n";
}

void BlockExp::print(std::ostream& out, int indent) const {
	indented(out, indent) << "BlockExp {\n";
	for (const Stm* stm : this->stms) {
		if (stm) stm->print(out, indent+1);
		else indented(out, indent+1) << "NULL\n";
	}
	indented(out, indent) << "}\n";
}
