#ifndef TOKEN_H
#define TOKEN_H

#include <string>

struct Token {
	enum Type {
		ERROR
	} type;

	std::string value;
};

#endif
