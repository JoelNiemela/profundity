#ifndef LEXER_H
#define LEXER_H

#include "token.h"

#include <string>
#include <cstddef> // std::size_t
#include <optional>

class Lexer {
	static const std::size_t buffer_size = 50;

private:
	std::string input;
	std::size_t pos;
	std::optional<Token> lookahead;

public:
	Lexer(std::string input);

	Token peak();
	Token pop();

private:
	Token next_token();
	Token::Type match_token(std::string token);

	char peak_char();
	char pop_char();
};

#endif
