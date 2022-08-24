#include "lexer.h"
#include "token.h"

#include <cctype>
#include <vector>
#include <utility> // pair
#include <regex>
#include <iostream>

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

bool Lexer::assert_token(Token found, Token::Type expected) {
	if (found.type != expected) {
		std::cerr << "Error: expected " << to_string(expected) << " found " << to_string(found.type) << std::endl;
		return false;
	}

	return true;
}

void Lexer::reset() {
	this->pos = 0;
	this->lookahead = std::nullopt;
}

void Lexer::debug() {
	Token t;
	do {
		t = this->pop();
		std::cout << to_string(t.type) << " " << t.value << std::endl;
	} while (t.type != Token::Type::ENDINPUT);
}

/*Token Lexer::next_token() {
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

	Token::Type type = match_token(str);
	if (type == Token::Type::WS) {
		return this->next_token();
	} else {
		return Token{type, str};
	}
}*/

bool Lexer::is_id_char(char c) {
	return is_id_char_head(c) || isdigit(c);
}

bool Lexer::is_id_char_head(char c) {
	return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || c == '_';
}

Token Lexer::next_token() {
	char c = pop_char();
	
	// literals
	if (is_id_char_head(c)) {
		std::string str(1, c);
		while (is_id_char(peak_char())) str += pop_char();

		// keywords
		if (str == "let") return Token{Token::LET, str};
		// literals
		else return Token{Token::ID, str};
	}
	if (isdigit(c)) {
		std::string str(1, c);
		while (isdigit(peak_char())) str += pop_char();
		if (peak_char() == '.') str += pop_char();
		while (isdigit(peak_char())) str += pop_char();

		return Token{Token::NUM, str};
	}
	if (c == '"') {
		std::string str = "";
		while ((c = pop_char()) != '"') {
			str += c;
			if (c == '\\') {
				str += pop_char();
			}
		}

		return Token{Token::STRING, str};
	}
	// brackets
	if (c == '(') return Token{Token::LPAR, "("};
	if (c == ')') return Token{Token::RPAR, ")"};
	if (c == '[') return Token{Token::LBRACKET, "["};
	if (c == ']') return Token{Token::RBRACKET, "]"};
	if (c == '{') return Token{Token::LBRACE, "{"};
	if (c == '}') return Token{Token::RBRACE, "}"};
	// arithmetic operators
	if (c == '*') return Token{Token::MUL, "*"};
	if (c == '/') return Token{Token::DIV, "/"};
	if (c == '+') return Token{Token::ADD, "+"};
	if (c == '-') {
		// type operators
		if (peak_char() == '>') {pop_char(); return Token{Token::ARROW, "->"};}
		// arithmetic operators
		else return Token{Token::SUB, "-"};
	}
	// logic operators
	if (c == '=') return Token{Token::EQ, "="};
	if (c == '<') {
		c = peak_char();
		if (c == '>') {pop_char(); return Token{Token::NE, "<>"};}
		if (c == '=') {pop_char(); return Token{Token::LE, "<="};}
		else return Token{Token::LT, "<"};
	}
	if (c == '>') {
		c = peak_char();
		if (c == '=') {pop_char(); return Token{Token::GE, ">="};}
		else return Token{Token::GT, ">"};
	}
	// type operators
	if (c == '?') return Token{Token::OPT, "?"};
	if (c == '^') return Token{Token::PTR, "^"};
	if (c == '&') return Token{Token::REF, "&"};
	// symbols
	if (c == ':') {
		if (peak_char() == '=') {pop_char(); return Token{Token::ASSIGN, ":="};}
		else return Token{Token::COLON, ":"};
	}
	if (c == ';') return Token{Token::SEMICOLON, ";"};
	if (c == ',') return Token{Token::COMMA, ","};
	if (c == '.') return Token{Token::DOT, "."};
	if (c == '|') return Token{Token::PIPE, "|"};
	// whitespace
	if (c == '\n') {
		int count = 1;
		while (peak_char() == '\n') pop_char(), count++;
		return Token{Token::NL, std::string(count, '\n')};
	}
	if (c == ' ') {
		while (peak_char() == ' ') pop_char();
		return next_token();
	}
	if (c == '\0') return Token{Token::ENDINPUT, "\0"};

	return Token{Token::ERROR, ""};
}

static const std::vector<std::pair<Token::Type, std::regex> > token_patterns = {
	// keywords
	{Token::Type::LET,       std::regex("let")},
	// literals
	{Token::Type::ID,        std::regex("[_a-zA-Z]+[_a-zA-Z0-9]*")},
	{Token::Type::NUM,       std::regex("[0-9]*\\.?[0-9]+")},
	{Token::Type::STRING,    std::regex("\"(?:[^\"\\\\]|\\\\.)*\"")},
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
	{Token::Type::PIPE,      std::regex("\\|")},
	{Token::Type::ASSIGN,    std::regex(":=")},
	// whitespace
	{Token::Type::NL,        std::regex("\\n+")},
	{Token::Type::WS,        std::regex(" +")},
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
