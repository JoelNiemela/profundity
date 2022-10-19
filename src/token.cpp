#include "token.h"

#include <iostream>

std::string to_string(Token::Type type) {
	switch (type) {
		// keywords
		case Token::LET:       return "LET";
		// literals
		case Token::ID:        return "ID";
		case Token::NUM:       return "NUM";
		case Token::STRING:    return "STRING";
		// brackets
		case Token::LPAR:      return "LPAR";
		case Token::RPAR:      return "RPAR";
		case Token::LBRACKET:  return "LBRACKET";
		case Token::RBRACKET:  return "RBRACKET";
		case Token::LBRACE:    return "LBRACE";
		case Token::RBRACE:    return "RBRACE";
		// arithmetic operators
		case Token::MUL:       return "MUL";
		case Token::DIV:       return "DIV";
		case Token::ADD:       return "ADD";
		case Token::SUB:       return "SUB";
		// logic operators
		case Token::EQ:        return "EQ";
		case Token::NE:        return "NE";
		case Token::LT:        return "LT";
		case Token::GT:        return "GT";
		case Token::LE:        return "LE";
		case Token::GE:        return "GE";
		// type operators
		case Token::ARROW:     return "ARROW";
		case Token::OPT:       return "OPT";
		case Token::PTR:       return "PTR";
		case Token::REF:       return "REF";
		case Token::ARRAY:       return "ARRAY";
		// symbols
		case Token::COLON:     return "COLON";
		case Token::SEMICOLON: return "SEMICOLON";
		case Token::DOT:       return "DOT";
		case Token::COMMA:     return "COMMA";
		case Token::PIPE:      return "PIPE";
		case Token::ASSIGN:    return "ASSIGN";
		case Token::THROUGH:   return "THROUGH";
		// indentation
		case Token::TAB:       return "TAB";
		case Token::END:       return "END";
		// whitespace
		case Token::NL:        return "NL";
		case Token::WS:        return "WS";
		// other
		case Token::ENDINPUT:  return "ENDINPUT";
		case Token::ERROR:     return "ERROR";
		default:
			std::cerr << "Error: Unknown token type in token.cpp:to_string" << std::endl;
			return "UNKNOWN";
	}
}
