#ifndef OP_H
#define OP_H

#include "token.h"

struct Op {
	enum Type {
		MUL,
		DIV,
		ADD,
		SUB,
		ARROW,
		NONE,
		ERROR,
	} type;

	enum Assoc {
		LEFT,
		RIGHT,
		INVALID,
	};

	static Assoc assoc(int prec);

	int prec();

	static Op from_token(Token::Type token);

	Op(Op::Type type);
	Op(Token token);
};

#endif
