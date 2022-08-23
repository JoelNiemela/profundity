#include "token.h"

#include <iostream>

std::string to_string(Token::Type type) {
	switch (type) {
		// keywords
		case Token::Type::LET:       return "LET";
		// literals
		case Token::Type::ID:        return "ID";
		case Token::Type::NUM:       return "NUM";
		case Token::Type::STRING:    return "STRING";
		// brackets
		case Token::Type::LPAR:      return "LPAR";
		case Token::Type::RPAR:      return "RPAR";
		case Token::Type::LBRACKET:  return "LBRACKET";
		case Token::Type::RBRACKET:  return "RBRACKET";
		case Token::Type::LBRACE:    return "LBRACE";
		case Token::Type::RBRACE:    return "RBRACE";
		// arithmetic operators
		case Token::Type::MUL:       return "MUL";
		case Token::Type::DIV:       return "DIV";
		case Token::Type::ADD:       return "ADD";
		case Token::Type::SUB:       return "SUB";
		// logic operators
		case Token::Type::EQ:        return "EQ";
		case Token::Type::NE:        return "NE";
		case Token::Type::LT:        return "LT";
		case Token::Type::GT:        return "GT";
		case Token::Type::LE:        return "LE";
		case Token::Type::GE:        return "GE";
		// type operators
		case Token::Type::ARROW:     return "ARROW";
		case Token::Type::OPT:       return "OPT";
		case Token::Type::PTR:       return "PTR";
		case Token::Type::REF:       return "REF";
		// symbols
		case Token::Type::COLON:     return "COLON";
		case Token::Type::SEMICOLON: return "SEMICOLON";
		case Token::Type::DOT:       return "DOT";
		case Token::Type::COMMA:     return "COMMA";
		case Token::Type::PIPE:      return "PIPE";
		case Token::Type::ASSIGN:    return "ASSIGN";
		// whitespace
		case Token::Type::NL:        return "NL";
		case Token::Type::WS:        return "WS";
		// other
		case Token::Type::ENDINPUT:  return "ENDINPUT";
		case Token::Type::ERROR:     return "ERROR";
		default:
			std::cerr << "Error: Unknown token type in token.cpp:to_string" << std::endl;
			return "UNKNOWN";
	}
}
