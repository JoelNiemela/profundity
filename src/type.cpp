#include "type.h"

std::ostream& indented(std::ostream& out, int indent) {
	for (int i = 0; i < indent; i++) {
		out << "    ";
	}

	return out;
}

void PrimitiveType::print(std::ostream& out, int indent) const {
	indented(out, indent) << "PrimitiveType {\n";
	indented(out, indent+1) << to_string(this->type) << "\n";
	indented(out, indent) << "}\n";
}

std::string to_string(PrimitiveType::Type type) {
	switch (type) {
		case PrimitiveType::I32:  return "I32";
		case PrimitiveType::TYPE: return "TYPE";
		default:
			std::cerr << "Error: Unknown type in type.cpp:to_string" << std::endl;
			return "UNKNOWN";
	}
}
