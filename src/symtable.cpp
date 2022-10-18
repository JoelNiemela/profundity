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
	if (auto it = symtable.find(id); it != symtable.end()) {
		return it->second;
	}

	if (parent) {
		return parent->lookup(id);
	}

	return std::nullopt;
}

void Symtable::insert(std::string id, Symbol symbol) {
	symtable[id] = symbol;
}
