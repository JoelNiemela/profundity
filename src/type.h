#ifndef TYPE_H
#define TYPE_H

#include <iostream>

struct Value {
	virtual ~Value() {}

	virtual void print(std::ostream& out, int indent=0) const = 0;
};

struct Type : Value {};

struct PrimitiveType : Type {
	enum Type {
		I32,
		TYPE,
	} type;

	PrimitiveType(Type type) : type(type) {}

	void print(std::ostream& out, int indent=0) const;
};


std::ostream& indented(std::ostream& out, int indent);

std::string to_string(PrimitiveType::Type type);

#endif
