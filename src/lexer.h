#ifndef LEXER_H
#define LEXER_H

#include "token.h"

#include <initializer_list>
#include <string>
#include <cstddef> // std::size_t
#include <optional>
#include <set>

class Lexer {
	static const std::size_t buffer_size = 50;

private:
	std::string input;
	std::size_t pos = 0;
	std::size_t line = 1;
	std::size_t column = 0;
	std::optional<Token> lookahead = std::nullopt;
	int line_indent = 0;
	int indent_diff = 0;
	bool at_start_of_line = true;

public:
	Lexer(std::string input);

	Token peak();
	Token pop();

	bool assert_token(Token found, Token::Type expected);
	bool assert_token(Token found, std::set<Token::Type> expected);

	void reset();
	void debug();

private:
	Token make_token(Token::Type type, std::string value);

	Token next_token();
	Token::Type match_token(std::string token);

	char peak_char();
	char pop_char();

	static bool is_id_char(char c);
	static bool is_id_char_head(char c);
};

#endif
