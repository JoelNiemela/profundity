#include "parser.h"
#include "ast.h"

Parser::Parser(Lexer &lexer) :
	lexer(lexer) {}

Program* Parser::parse() {
	return this->parse_program();
}

Program* Parser::parse_program() {
	Stm* stm;
	std::vector<Stm*> stms;
	do {
		stm = parse_stm();
		if (stm) stms.push_back(stm);
	} while (stm);

	return new Program(stms);
}

Stm* Parser::parse_stm() {
	Token token = this->lexer.peak();

	switch (token.type) {
		case Token::Type::LET:
			return parse_let_stm();
		default:
			std::cerr << "Syntax Error (Stm)" << std::endl;
			return nullptr;
	}
}

LetStm* Parser::parse_let_stm() {
	lexer.assert_token(lexer.pop(), Token::Type::LET);

	Token id = lexer.pop();
	lexer.assert_token(id, Token::Type::ID);

	lexer.assert_token(lexer.pop(), Token::Type::EQ);

	Exp* exp = parse_exp();

	return new LetStm(id.value, exp);
}

Exp* Parser::parse_exp(int prec) {
	if (prec == 0) {
		return parse_exp_atom();
	}

	if (Op::assoc(prec) == Op::Assoc::LEFT) {
		Exp* lexp = parse_exp(prec-1);
		while (Op(lexer.peak()).prec() == prec) {
			Op op = lexer.pop();
			Exp* rexp = parse_exp(prec-1);
			lexp = new OpExp(lexp, rexp, op);
		}

		return lexp;
	} else if (Op::assoc(prec) == Op::Assoc::RIGHT) {
		Exp* lexp = parse_exp(prec-1);
		if (Op(lexer.peak()).prec() == prec) {
			Op op = lexer.pop();
			Exp* rexp = parse_exp(prec);
			lexp = new OpExp(lexp, rexp, op);
		}

		return lexp;
	} else {
		std::cerr << "Syntax Error (Exp)" << std::endl;
		return nullptr;
	}
}

Exp* Parser::parse_exp_atom() {
	Token token = lexer.pop();

	switch (token.type) {
		case Token::Type::NUM:
			return new NumExp(token.value);
		default:
			std::cerr << "Syntax Error (Exp)" << std::endl;
			return nullptr;
	}
}
