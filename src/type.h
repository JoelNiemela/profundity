#ifndef TYPE_H
#define TYPE_H

#include <iostream>

struct Type;

struct Value {
	Type* type;

	Value(Type* type) : type(type) {}
	virtual ~Value() {}

	virtual void print(std::ostream& out, int indent=0) const = 0;
};

struct Type {
	virtual ~Type() {}

	virtual void print(std::ostream& out, int indent=0) const = 0;
};

struct TypeValue : Value {
	Type* value_type;

	TypeValue(Type* type);

	void print(std::ostream& out, int indent=0) const;
};

struct ComptimeType : Type {
	enum Type {
		TYPE,
		UNIT,
		NUM,
	} type_variant;

	ComptimeType(Type type) : type_variant(type) {}

	void print(std::ostream& out, int indent=0) const;
};

struct PrimitiveType : Type {
	enum Type {
		I32,
		STR,
	} type_variant;

	PrimitiveType(Type type) : type_variant(type) {}

	void print(std::ostream& out, int indent=0) const;
};

struct FunctionType : Type {
	Type* arg_type;
	Type* res_type;

	FunctionType(Type* arg_type, Type* res_type) : arg_type(arg_type), res_type(res_type) {}

	void print(std::ostream& out, int indent=0) const;
};

struct RecordType : Type {
	std::string id;
	Type* type;

	RecordType(std::string id, Type* type) : id(id), type(type) {}

	void print(std::ostream& out, int indent=0) const;
};

struct ComptimeValue : Value {
	std::string value;

	ComptimeValue(ComptimeType* type, std::string value) : Value(type), value(value) {}
	ComptimeValue(ComptimeType::Type type, std::string value) : Value(new ComptimeType(type)), value(value) {}

	void print(std::ostream& out, int indent=0) const;
};

struct PrimitiveValue : Value {
	std::string value;

	PrimitiveValue(PrimitiveType* type, std::string value) : Value(type), value(value) {}
	PrimitiveValue(PrimitiveType::Type type, std::string value) : Value(new PrimitiveType(type)), value(value) {}

	void print(std::ostream& out, int indent=0) const;
};

std::ostream& indented(std::ostream& out, int indent);

std::string to_string(ComptimeType::Type type);
std::string to_string(PrimitiveType::Type type);

#endif
