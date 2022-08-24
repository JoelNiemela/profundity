#ifndef AST_H
#define AST_H

#include "op.h"

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
	Exp* type_exp;
	Exp* exp;

	LetStm(std::string id, Exp* type_exp, Exp* exp) : id(id), type_exp(type_exp), exp(exp) {}

	void print(std::ostream& out, int indent=0) const;
};

struct UnitExp : Exp {
	UnitExp() {}

	void print(std::ostream& out, int indent=0) const;
};

struct NumExp : Exp {
	std::string num;

	NumExp(std::string num) : num(num) {}

	void print(std::ostream& out, int indent=0) const;
};

struct VarExp : Exp {
	std::string id;

	VarExp(std::string id) : id(id) {}

	void print(std::ostream& out, int indent=0) const;
};

struct RecordExp : Exp {
	std::string id;
	Exp* type_exp;

	RecordExp(std::string id, Exp* type_exp) : id(id), type_exp(type_exp) {}

	void print(std::ostream& out, int indent=0) const;
};

struct OpExp : Exp {
	Exp* lexp;
	Exp* rexp;
	Op op;

	OpExp(Exp* lexp, Exp* rexp, Op op) : lexp(lexp), rexp(rexp), op(op) {}

	void print(std::ostream& out, int indent=0) const;
};

#endif
