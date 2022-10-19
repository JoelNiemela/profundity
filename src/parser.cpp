#include "parser.h"

#include "ast.h"
#include "type.h"

Parser::Parser(Lexer &lexer) :
	lexer(lexer),
	symtable(new Symtable()),
	comptime_state(new ComptimeState())
{
	symtable->insert("i32", Symbol(new ComptimeType(ComptimeType::TYPE), new TypeValue(new PrimitiveType(PrimitiveType::I32))));
	symtable->insert("type", Symbol(new ComptimeType(ComptimeType::TYPE), new TypeValue(new ComptimeType(ComptimeType::TYPE))));
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
			if (Exp* exp; (exp = this->parse_exp())) {
				lexer.assert_token(lexer.pop(), Token::NL);
				return new ExpStm(exp);
			}

			std::cerr << "Syntax Error (Stm) at " << token.line << ":" << token.column
					  << ": Expected LET, found " << to_string(token.type) << std::endl;
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

	ValueExp* type;
	TypeValue* type_value;
	if ((type = dynamic_cast<ValueExp*>(type_exp)) && (type_value = dynamic_cast<TypeValue*>(type->value))) {
		ValueExp* value_exp;
		if ((value_exp = dynamic_cast<ValueExp*>(exp))) {
			symtable->insert(id.value, Symbol(type_value->value_type, value_exp->value));
			return new LetStm(id.value, type_value->value_type, value_exp);
		} else {
			symtable->insert(id.value, Symbol(type_value->value_type));
			return new LetStm(id.value, type_value->value_type, exp);
		}
	} else {
		Value* value = this->comptime_state->run_exp(type_exp);
		if (TypeValue* type_value; (type_value = dynamic_cast<TypeValue*>(value))) {
			symtable->insert(id.value, Symbol(type_value->value_type, type_value));
			return new LetStm(id.value, type_value->value_type, exp);
		}
	}

	ValueExp* value_exp;
	if ((value_exp = dynamic_cast<ValueExp*>(exp))) {
		Type* type = value_exp->value->type;
		if ((value_exp = dynamic_cast<ValueExp*>(exp))) {
			symtable->insert(id.value, Symbol(type, value_exp->value));
			return new LetStm(id.value, type, value_exp);
		} else {
			symtable->insert(id.value, Symbol(type));
			return new LetStm(id.value, type, exp);
		}
	}

	std::cerr << "Error at " << id.line << ":" << id.column << ": type must be known at compile time" << std::endl;
	type_exp->print(std::cerr);
	return nullptr;
}

Exp* Parser::parse_exp(int prec) {
	if (prec == 0) {
		return parse_exp_atom();
	}

	if (std::optional<Op> op; (op = Op::from_token(lexer.peak())) && op->prec() == prec && Op::unary(prec)) {
		lexer.pop();
		return new OpExp(nullptr, parse_exp(prec), *op);
	} else { 
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
		}
	}

	Token token = lexer.peak();
	std::cerr << "Syntax Error (Exp) at " << token.line << ":" << token.column << ": Invalid operator" << std::endl;
	return nullptr;
}

Exp* Parser::parse_exp_atom() {
	Token token = lexer.pop();

	switch (token.type) {
		case Token::NUM:
			//return new NumExp(token.value);
			return new ValueExp(new ComptimeValue(ComptimeType::NUM, token.value));
		case Token::STRING:
			//return new StrExp(token.value);
			return new ValueExp(new PrimitiveValue(PrimitiveType::STR, token.value));
		case Token::ID: {
			std::string id = token.value;
			if (lexer.peak().type == Token::COLON) {
				lexer.pop();
				Exp* type_exp = parse_exp();
				Value* value = this->comptime_state->run_exp(type_exp);
				if (TypeValue* type_value; (type_value = dynamic_cast<TypeValue*>(value))) {
					return new ValueExp(new TypeValue(new RecordType(id, type_value->value_type)));
				} else {
					return new RecordExp(id, type_exp);
				}
			} else {
				std::optional<Symbol> symbol = symtable->lookup(id);
				if (symbol && symbol->value) {
					return new ValueExp(symbol->value);
				} else {
					return new VarExp(id);
				}
			}
		}
		case Token::LPAR: {
			if (lexer.peak().type == Token::RPAR) {
				lexer.pop();
				return new UnitExp();
				//return new ValueExp(new ComptimeValue(ComptimeType::UNIT, "()"));
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
			std::cerr << "Syntax Error (Exp) at " << token.line << ":" << token.column << std::endl;
			return nullptr;
	}
}
