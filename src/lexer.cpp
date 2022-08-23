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

	if (str[0] == '\0') {
		return Token{Token::Type::ENDINPUT};
	}

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
	// keywords
	{Token::Type::LET,       std::regex("let")},
	// literals
	{Token::Type::ID,        std::regex("[_a-zA-Z]+[_a-zA-Z0-9]*")},
	{Token::Type::NUM,       std::regex("[0-9]*\\.?[0-9]+")},
	{Token::Type::STRING,    std::regex("/\"(?:[^\"\\\\]|\\\\.)*\"/")},
	// brackets
	{Token::Type::LPAR,      std::regex("\\(")},
	{Token::Type::RPAR,      std::regex("\\)")},
	{Token::Type::LBRACKET,  std::regex("\\[")},
	{Token::Type::RBRACKET,  std::regex("\\]")},
	{Token::Type::LBRACE,    std::regex("\\{")},
	{Token::Type::RBRACE,    std::regex("\\}")},
	// arithmetic operators
	{Token::Type::MUL,       std::regex("\\*")},
	{Token::Type::DIV,       std::regex("/")},
	{Token::Type::ADD,       std::regex("\\+")},
	{Token::Type::SUB,       std::regex("\\-")},
	// logic operators
	{Token::Type::EQ,        std::regex("=")},
	{Token::Type::NE,        std::regex("<>")},
	{Token::Type::LT,        std::regex("<")},
	{Token::Type::GT,        std::regex(">")},
	{Token::Type::LE,        std::regex("<=")},
	{Token::Type::GE,        std::regex(">=")},
	// type operators
	{Token::Type::ARROW,     std::regex("\\->")},
	{Token::Type::OPT,       std::regex("\\?")},
	{Token::Type::PTR,       std::regex("\\^")},
	{Token::Type::REF,       std::regex("&")},
	// symbols
	{Token::Type::COLON,     std::regex(":")},
	{Token::Type::SEMICOLON, std::regex(";")},
	{Token::Type::COMMA,     std::regex(",")},
	{Token::Type::DOT,       std::regex("\\.")},
	{Token::Type::PIPE,      std::regex("|")},
	{Token::Type::ASSIGN,    std::regex(":=")},
	// whitespace
	{Token::Type::NL,        std::regex("\\n+")},
	{Token::Type::WS,        std::regex("[\\s]*")},
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