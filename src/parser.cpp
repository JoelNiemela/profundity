#include "parser.h"

#include "ast.h"
#include "symtable.h"
#include "comptime.h"

Parser::Parser(Lexer &lexer) :
	lexer(lexer),
	symtable(new Symtable())
{
	symtable->insert("i32", Symbol(new PrimitiveType(PrimitiveType::TYPE), new PrimitiveType(PrimitiveType::I32)));
	symtable->insert("type", Symbol(new PrimitiveType(PrimitiveType::TYPE), new PrimitiveType(PrimitiveType::TYPE)));
}

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
		case Token::Type::END:
		case Token::Type::ENDINPUT:
			return nullptr;
		default:
			std::cerr << "Syntax Error (Stm): Expected LET, found " << to_string(token.type) << std::endl;
			return nullptr;
	}
}

LetStm* Parser::parse_let_stm() {
	lexer.assert_token(lexer.pop(), Token::Type::LET);

	Token id = lexer.pop();
	lexer.assert_token(id, Token::Type::ID);

	Exp* type_exp = nullptr;
	if (lexer.peak().type == Token::Type::COLON) {
		lexer.pop();
		type_exp = parse_exp(14);
	}

	lexer.assert_token(lexer.pop(), Token::Type::EQ);

	Exp* exp = parse_exp(14);

	lexer.assert_token(lexer.pop(), {Token::NL, Token::END});

	ComptimeEngine comptime_engine(symtable);
	Type* type = comptime_engine.eval_type_exp(type_exp);
	symtable->insert(id.value, Symbol(type));

	return new LetStm(id.value, type, exp);
}

Exp* Parser::parse_exp(int prec) {
	if (prec == 0) {
		return parse_exp_atom();
	}

	if (Op::assoc(prec) == Op::Assoc::LEFT) {
		Exp* lexp = parse_exp(prec-1);

		std::optional<Op> op;
		while ((op = Op::from_token(lexer.peak())) && op->prec() == prec) {
			lexer.pop();
			Exp* rexp = parse_exp(prec-1);
			lexp = new OpExp(lexp, rexp, *op);
		}

		return lexp;
	} else if (Op::assoc(prec) == Op::Assoc::RIGHT) {
		Exp* lexp = parse_exp(prec-1);

		std::optional<Op> op;
		if ((op = Op::from_token(lexer.peak())) && op->prec() == prec) {
			lexer.pop();
			Exp* rexp = parse_exp(prec);
			lexp = new OpExp(lexp, rexp, *op);
		}

		return lexp;
	} else {
		std::cerr << "Syntax Error (Exp): Invalid operator" << std::endl;
		return nullptr;
	}
}

Exp* Parser::parse_exp_atom() {
	Token token = lexer.pop();

	switch (token.type) {
		case Token::NUM:
			return new NumExp(token.value);
		case Token::ID: {
			std::string id = token.value;
			if (lexer.peak().type == Token::COLON) {
				lexer.pop();
				Exp* type_exp = parse_exp();
				return new RecordExp(id, type_exp);
			} else {
				return new VarExp(id);
			}
		}
		case Token::LPAR: {
			if (lexer.peak().type == Token::RPAR) {
				lexer.pop();
				return new UnitExp();
			} else {
				Exp* exp = parse_exp();
				lexer.assert_token(lexer.pop(), Token::RPAR);
				return exp;
			}
		}
		case Token::NL: {
			lexer.assert_token(lexer.pop(), Token::TAB);

			symtable = symtable->push_block();

			Stm* stm;
			std::vector<Stm*> stms;
			do {
				stm = parse_stm();
				if (stm) stms.push_back(stm);
			} while (stm);
			lexer.assert_token(lexer.peak(), Token::END);

			symtable = symtable->pop_block();

			return new BlockExp(stms);
		}
		default:
			std::cerr << "Syntax Error (Exp)" << std::endl;
			return nullptr;
	}
}
