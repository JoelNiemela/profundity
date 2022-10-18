#include "type.h"

TypeValue::TypeValue(Type* type) : Value(new ComptimeType(ComptimeType::TYPE)), value_type(type) {}

std::ostream& indented(std::ostream& out, int indent) {
	for (int i = 0; i < indent; i++) {
		out << "    ";
	}

	return out;
}

void TypeValue::print(std::ostream& out, int indent) const {
	indented(out, indent) << "TypeValue {\n";
	this->type->print(out, indent+1);
	indented(out, indent) << "}\n";
}

void ComptimeType::print(std::ostream& out, int indent) const {
	indented(out, indent) << "ComptimeType {\n";
	indented(out, indent+1) << to_string(this->type_variant) << "\n";
	indented(out, indent) << "}\n";
}

void PrimitiveType::print(std::ostream& out, int indent) const {
	indented(out, indent) << "PrimitiveType {\n";
	indented(out, indent+1) << to_string(this->type_variant) << "\n";
	indented(out, indent) << "}\n";
}

void FunctionType::print(std::ostream& out, int indent) const {
	indented(out, indent) << "FunctionType {\n";
	this->arg_type->print(out, indent+1);
	this->res_type->print(out, indent+1);
	indented(out, indent) << "}\n";
}

void ComptimeValue::print(std::ostream& out, int indent) const {
	indented(out, indent) << "ComptimeValue {\n";
	indented(out, indent+1) << this->value << "\n";
	this->type->print(out, indent+1);
	indented(out, indent) << "}\n";
}

void PrimitiveValue::print(std::ostream& out, int indent) const {
	indented(out, indent) << "PrimitiveValue {\n";
	indented(out, indent+1) << this->value << "\n";
	this->type->print(out, indent+1);
	indented(out, indent) << "}\n";
}

std::string to_string(ComptimeType::Type type) {
	switch (type) {
		case ComptimeType::TYPE: return "TYPE";
		case ComptimeType::UNIT: return "UNIT";
		case ComptimeType::NUM:  return "NUM";
		default:
			std::cerr << "Error: Unknown type in type.cpp:to_string" << std::endl;
			return "UNKNOWN";
	}
}

std::string to_string(PrimitiveType::Type type) {
	switch (type) {
		case PrimitiveType::I32:  return "I32";
		default:
			std::cerr << "Error: Unknown type in type.cpp:to_string" << std::endl;
			return "UNKNOWN";
	}
}
