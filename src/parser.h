#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "ast.h"

class Parser {
private:
	Lexer &lexer;
public:
	Parser(Lexer &lexer);

	Program* parse();
private:
	Program* parse_program();
	Stm* parse_stm();
	LetStm* parse_let_stm();
	Exp* parse_exp(int prec=15);
	Exp* parse_exp_atom();
};

#endif
