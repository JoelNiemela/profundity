#include "op.h"

#include <iostream>

Op::Op(Op::Type type) : type(type) {}

std::optional<Op> Op::from_token(Token token) {
	switch (token.type) {
		case Token::Type::MUL: return Op(Op::MUL);
		case Token::Type::DIV: return Op(Op::DIV);
		case Token::Type::ADD: return Op(Op::ADD);
		case Token::Type::SUB: return Op(Op::SUB);
		default: return std::nullopt;
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
