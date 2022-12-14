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
		CONCAT,
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
		ARRAY,
		COMMA,
	} type;

	enum Assoc {
		LEFT,
		RIGHT,
		INVALID,
	};

	static Assoc assoc(int prec);
	static bool unary(int prec);
	static bool list(int prec);

	int prec();

	static std::optional<Op> from_token(Token token);

	Op(Op::Type type);
};

std::string to_string(Op::Type type);

#endif
