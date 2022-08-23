#include "lexer.h"

#include <vector>
#include <utility> // pair
#include <regex>

Lexer::Lexer(std::string input) :
	input(input),
	pos(0) {}

Token Lexer::peak() {
	if (!this->lookahead) {
		this->lookahead = this->next_token();
	}

	return *this->lookahead;
}

Token Lexer::pop() {
	if (this->lookahead) {
		Token t = *this->lookahead;
		this->lookahead = std::nullopt;
		return t;
	} else {
		return this->next_token();
	}
}

Token Lexer::next_token() {
	std::string str = "";
	str += peak_char();

	while (match_token(str) == Token::Type::ERROR) {
		pop_char();
		str += peak_char();
	}

	while (match_token(str) != Token::Type::ERROR) {
		pop_char();
		str += peak_char();
	}

	str.pop_back();

	return Token{match_token(str), str};
}

static const std::vector<std::pair<Token::Type, std::regex> > token_patterns = {
	{}
};

Token::Type Lexer::match_token(std::string token) {
	for (const auto& [type, pattern] : token_patterns) {
		if (std::regex_match(token.begin(), token.end(), pattern)) {
			return type;
		}
	}

	return Token::Type::ERROR;
}

char Lexer::peak_char() {
	if (this->pos < this->input.size()) {
		return this->input[this->pos];
	} else {
		return '\0';
	}
}

char Lexer::pop_char() {
	char c = this->peak_char();
	this->pos++;
	return c;
}
