#include "symtable.h"

Symtable::Symtable() : parent(nullptr) {}

Symtable::Symtable(Symtable* parent) : parent(parent) {}

Symtable* Symtable::push_block() {
	return new Symtable(this);
}

Symtable* Symtable::pop_block() {
	return parent;
}

#include <iostream>

std::optional<Symbol> Symtable::lookup(std::string id) {
	auto it = symtable.find(id);
	if (it != symtable.end()) {
		return it->second;
	}

	if (parent) {
		std::optional<Symbol> symbol = parent->lookup(id);
		if (symbol) return symbol;
	}

	return std::nullopt;
}

void Symtable::insert(std::string id, Symbol symbol) {
	symtable[id] = symbol;
}
