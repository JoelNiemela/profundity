#include "op.h"

#include <iostream>

Op::Op(Op::Type type) : type(type) {}

std::optional<Op> Op::from_token(Token token) {
	switch (token.type) {
		case Token::Type::MUL:    return Op(Op::MUL);
		case Token::Type::DIV:    return Op(Op::DIV);
		case Token::Type::ADD:    return Op(Op::ADD);
		case Token::Type::SUB:    return Op(Op::SUB);
		case Token::Type::CONCAT: return Op(Op::CONCAT);
		case Token::Type::EQ:     return Op(Op::EQ);
		case Token::Type::NE:     return Op(Op::NE);
		case Token::Type::LT:     return Op(Op::LT);
		case Token::Type::GT:     return Op(Op::GT);
		case Token::Type::LE:     return Op(Op::LE);
		case Token::Type::GE:     return Op(Op::GE);
		case Token::Type::ARROW:  return Op(Op::ARROW);
		case Token::Type::OPT:    return Op(Op::OPT);
		case Token::Type::PTR:    return Op(Op::PTR);
		case Token::Type::REF:    return Op(Op::REF);
		case Token::Type::ARRAY:  return Op(Op::ARRAY);
		case Token::Type::COMMA:  return Op(Op::COMMA);
		default: return std::nullopt;
	}
}

Op::Assoc Op::assoc(int prec) {
	switch (prec) {
		case 0:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
		case 11:
		case 12:
		case 13:
		case 15:
			return Op::Assoc::LEFT;
		case 1:
		case 10:
			return Op::Assoc::RIGHT;
		case 14:
		default:
			return Op::Assoc::INVALID;
	}
}

bool Op::unary(int prec) {
	switch (prec) {
		case 0:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
		case 10:
		case 11:
		case 12:
		case 13:
		case 14:
		case 15:
			return false;
		case 1:
			return true;
		default:
			std::cerr << "Error: Unknown precedence level in ast.cpp:Op::unary" << std::endl;
			return false;
	}
}

bool Op::list(int prec) {
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
		case 11:
		case 12:
		case 13:
		case 15:
			return false;
		case 14:
			return true;
		default:
			std::cerr << "Error: Unknown precedence level in ast.cpp:Op::list" << std::endl;
			return false;
	}
}

int Op::prec() {
	switch (this->type) {
		case Op::Type::MUL:    return 5;
		case Op::Type::DIV:    return 5;
		case Op::Type::ADD:    return 6;
		case Op::Type::SUB:    return 6;
		case Op::Type::CONCAT: return 6;
		case Op::Type::EQ:     return 15;
		case Op::Type::NE:     return 15;
		case Op::Type::LT:     return 13;
		case Op::Type::GT:     return 13;
		case Op::Type::LE:     return 13;
		case Op::Type::GE:     return 13;
		case Op::Type::ARROW:  return 10;
		case Op::Type::OPT:    return 1;
		case Op::Type::PTR:    return 1;
		case Op::Type::REF:    return 1;
		case Op::Type::ARRAY:  return 1;
		case Op::Type::COMMA:  return 14;
		default:
			std::cerr << "Error: Unknown operator in ast.cpp:Op::prec" << std::endl;
			return -1;
	}
}

std::string to_string(Op::Type type) {
	switch (type) {
		case Op::Type::MUL:    return "MUL";
		case Op::Type::DIV:    return "DIV";
		case Op::Type::ADD:    return "ADD";
		case Op::Type::SUB:    return "SUB";
		case Op::Type::CONCAT: return "CONCAT";
		case Op::Type::EQ:     return "EQ";
		case Op::Type::NE:     return "NE";
		case Op::Type::LT:     return "LT";
		case Op::Type::GT:     return "GT";
		case Op::Type::LE:     return "LE";
		case Op::Type::GE:     return "GE";
		case Op::Type::ARROW:  return "ARROW";
		case Op::Type::OPT:    return "OPT";
		case Op::Type::PTR:    return "PTR";
		case Op::Type::REF:    return "REF";
		case Op::Type::ARRAY:  return "ARRAY";
		case Op::Type::COMMA:  return "COMMA";
		default:
			std::cerr << "Error: Unknown operator in op.cpp:to_string" << std::endl;
			return "UNKNOWN";
	}
}
