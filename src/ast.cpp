#include "ast.h"

std::ostream& indented(std::ostream& out, int indent) {
	for (int i = 0; i < indent; i++) {
		out << "    ";
	}

	return out;
}

void Program::print(std::ostream& out, int indent) const {
	indented(out, indent) << "Program {\n";
	for (const Stm* stm : this->stms) {
		if (stm) stm->print(out, indent+1);
		else indented(out, indent+1) << "NULL\n";
	}
	indented(out, indent) << "}\n";
}

void LetStm::print(std::ostream& out, int indent) const {
	indented(out, indent) << "LetStm {\n";
	indented(out, indent+1) << this->id << "\n";
	if (this->type_exp) this->type_exp->print(out, indent+1);
	else indented(out, indent+1) << "NULL\n";
	if (this->exp) this->exp->print(out, indent+1);
	else indented(out, indent+1) << "NULL\n";
	indented(out, indent) << "}\n";
}

void UnitExp::print(std::ostream& out, int indent) const {
	indented(out, indent) << "UnitExp {}\n";
}

void NumExp::print(std::ostream& out, int indent) const {
	indented(out, indent) << "NumExp {\n";
	indented(out, indent+1) << this->num << "\n";
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

