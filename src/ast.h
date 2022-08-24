#ifndef AST_H
#define AST_H

#include "token.h"

#include <iostream>
#include <vector>

std::ostream& indented(std::ostream& out, int indent);

struct Node {
	virtual ~Node() {}

	virtual void print(std::ostream& out, int indent=0) const = 0;
};

struct Stm : Node {};
struct Exp : Node {};

struct Program : Node {
	std::vector<Stm*> stms;

	Program(std::vector<Stm*> stms) : stms(stms) {}

	void print(std::ostream& out, int indent=0) const;
};

struct LetStm : Stm {
	std::string id;
	Exp* exp;

	LetStm(std::string id, Exp* exp) : id(id), exp(exp) {}

	void print(std::ostream& out, int indent=0) const;
};

struct NumExp : Exp {
	std::string num;

	NumExp(std::string num) : num(num) {}

	void print(std::ostream& out, int indent=0) const;
};

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

struct OpExp : Exp {
	Exp* lexp;
	Exp* rexp;
	Op op;

	OpExp(Exp* lexp, Exp* rexp, Op op) : lexp(lexp), rexp(rexp), op(op) {}

	void print(std::ostream& out, int indent=0) const;
};

#endif
