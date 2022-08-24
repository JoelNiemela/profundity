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
	if (this->exp) this->exp->print(out, indent+1);
	else indented(out, indent+1) << "NULL\n";
	indented(out, indent) << "}\n";
}

void NumExp::print(std::ostream& out, int indent) const {
	indented(out, indent) << "NumExp {\n";
	indented(out, indent+1) << this->num << "\n";
	indented(out, indent) << "}\n";
}

void OpExp::print(std::ostream& out, int indent) const {
	indented(out, indent) << "OpExp {\n";
	if (this->lexp) this->lexp->print(out, indent+1);
	else indented(out, indent+1) << "NULL\n";
	if (this->rexp) this->rexp->print(out, indent+1);
	else indented(out, indent+1) << "NULL\n";
	indented(out, indent+1) << this->op.type << "\n";
	indented(out, indent) << "}\n";
}

Op::Op(Op::Type type) : type(type) {}

Op::Op(Token token) : Op(from_token(token.type)) {}

Op Op::from_token(Token::Type token) {
	switch (token) {
		case Token::Type::MUL: return Op(Op::MUL);
		case Token::Type::DIV: return Op(Op::DIV);
		case Token::Type::ADD: return Op(Op::ADD);
		case Token::Type::SUB: return Op(Op::SUB);
		case Token::Type::ENDINPUT: return Op(Op::NONE);
		default:
			std::cerr << "Error: invalid operator " << to_string(token) << std::endl;
			return Op(Op::ERROR);
	}
}

Op::Assoc Op::assoc(int prec) {
	switch (prec) {
		case 0:
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
		case 10:
			return Op::Assoc::LEFT;
			return Op::Assoc::RIGHT;
		default:
			return Op::Assoc::INVALID;
	}
}

int Op::prec() {
	switch (this->type) {
		case Op::Type::MUL: return 10;
		case Op::Type::DIV: return 10;
		case Op::Type::ADD: return 10;
		case Op::Type::SUB: return 10;
		case Op::Type::ARROW: return 10;
		default:
			std::cerr << "Error: Unknown operator in ast.cpp:Op::prec" << std::endl;
			return -1;
	}
}
