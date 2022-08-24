#ifndef SYMTABLE_H
#define SYMTABLE_H

#include "type.h"

#include <string>
#include <unordered_map>
#include <optional>

struct Symbol {
	Type* type;
	Value* value;

	Symbol() : type(nullptr), value(nullptr) {}
	Symbol(Type* type) : type(type), value(nullptr) {}
	Symbol(Type* type, Value* value) : type(type), value(value) {}
};

class Symtable {
private:
	std::unordered_map<std::string, Symbol> symtable;
	Symtable* parent;
public:
	Symtable();
	Symtable(Symtable* parent);

	Symtable* push_block();
	Symtable* pop_block();

	std::optional<Symbol> lookup(std::string id);
	void insert(std::string id, Symbol);
};

#endif
