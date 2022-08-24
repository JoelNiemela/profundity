#ifndef OP_H
#define OP_H

#include "token.h"

#include <optional>

struct Op {
	enum Type {
		MUL,
		DIV,
		ADD,
		SUB,
		EQ,
		NE,
		LT,
		GT,
		LE,
		GE,
		ARROW,
		OPT,
		PTR,
		REF,
	} type;

	enum Assoc {
		LEFT,
		RIGHT,
		INVALID,
	};

	static Assoc assoc(int prec);

	int prec();

	static std::optional<Op> from_token(Token token);

	Op(Op::Type type);
};

std::string to_string(Op::Type type);

#endif
