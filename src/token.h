#ifndef TOKEN_H
#define TOKEN_H

#include <string>

struct Token {
	enum Type {
		// keywords
		LET,
		// literals
		ID,
		NUM,
		STRING,
		// brackets
		LPAR, RPAR,
		LBRACKET, RBRACKET,
		LBRACE, RBRACE,
		// arithmetic operators
		MUL,
		DIV,
		ADD,
		SUB,
		// logic operators
		EQ, NE,
		LT, GT,
		LE, GE,
		// type operators
		ARROW,
		OPT,
		PTR,
		REF,
		ARRAY,
		// symbols
		COLON,
		SEMICOLON,
		DOT,
		COMMA,
		PIPE,
		ASSIGN,
		THROUGH,
		// indentation
		TAB,
		END,
		// whitespace
		NL,
		WS,
		// other
		ENDINPUT,
		ERROR,
	} type;

	std::string value;

	std::size_t pos;
	std::size_t line;
	std::size_t column;
};

std::string to_string(Token::Type);

#endif
