#include "lexer.h"
#include "token.h"

#include <cctype>
#include <string>
#include <vector>
#include <utility> // std::pair
#include <regex>
#include <iostream>
#include <numeric>

Lexer::Lexer(std::string input) : input(input) {}

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
	return assert_token(found, std::set{expected});
}

bool Lexer::assert_token(Token found, std::set<Token::Type> expected) {
	if (expected.empty()) {
		std::cerr << "Error: found token " << to_string(found.type) << " in unreachable.";
		return false;
	}

	if (!expected.contains(found.type)) {
		std::string expected_str = std::accumulate(next(expected.begin()), expected.end(), to_string(*expected.begin()),
			[](std::string str, Token::Type token) {
				return str + ", " + to_string(token);
			}
		);
		std::cerr << "Error: expected " << expected_str << ", found " << to_string(found.type) << std::endl;
		return false;
	}

	return true;
}

void Lexer::reset() {
	this->pos = 0;
	this->line = 0;
	this->column = 0;
	this->lookahead = std::nullopt;
	this->line_indent = 0;
	this->indent_diff = 0;
	this->at_start_of_line = true;
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
		return this->make_token(Token::Type::ENDINPUT);
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
		return this->make_token(type, str);
	}
}*/

bool Lexer::is_id_char(char c) {
	return is_id_char_head(c) || isdigit(c);
}

bool Lexer::is_id_char_head(char c) {
	return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || c == '_';
}

Token Lexer::make_token(Token::Type type, std::string value) {
	return Token{
		type,
		value,
		this->pos,
		this->line,
		this->column,
	};
}

Token Lexer::next_token() {
	// indentation
	if (at_start_of_line) {
		if (indent_diff > 0) {
			indent_diff--;
			return this->make_token(Token::TAB, "\t");
		} else if (indent_diff < 0) {
			indent_diff++;
			return this->make_token(Token::END, "");
		}

		at_start_of_line = false;
	}

	char c = pop_char();
	
	// literals
	if (is_id_char_head(c)) {
		std::string str(1, c);
		while (is_id_char(peak_char())) str += pop_char();

		// keywords
		if (str == "let") return this->make_token(Token::LET, str);
		// literals
		else return this->make_token(Token::ID, str);
	}
	if (isdigit(c)) {
		std::string str(1, c);
		while (isdigit(peak_char())) str += pop_char();
		if (peak_char() == '.') str += pop_char();
		while (isdigit(peak_char())) str += pop_char();

		return this->make_token(Token::NUM, str);
	}
	if (c == '"') {
		std::string str = "";
		while ((c = pop_char()) != '"') {
			str += c;
			if (c == '\\') {
				str += pop_char();
			}
		}

		return this->make_token(Token::STRING, str);
	}
	// brackets
	if (c == '(') return this->make_token(Token::LPAR, "(");
	if (c == ')') return this->make_token(Token::RPAR, ")");
	if (c == '[') return this->make_token(Token::LBRACKET, "[");
	if (c == ']') return this->make_token(Token::RBRACKET, "]");
	if (c == '{') return this->make_token(Token::LBRACE, "{");
	if (c == '}') return this->make_token(Token::RBRACE, "}");
	// arithmetic operators
	if (c == '*') return this->make_token(Token::MUL, "*");
	if (c == '/') return this->make_token(Token::DIV, "/");
	if (c == '+') return this->make_token(Token::ADD, "+");
	if (c == '-') {
		// type operators
		if (peak_char() == '>') {pop_char(); return this->make_token(Token::ARROW, "->");}
		// arithmetic operators
		else return this->make_token(Token::SUB, "-");
	}
	// logic operators
	if (c == '=') return this->make_token(Token::EQ, "=");
	if (c == '<') {
		c = peak_char();
		if (c == '>') {pop_char(); return this->make_token(Token::NE, "<>");}
		if (c == '=') {pop_char(); return this->make_token(Token::LE, "<=");}
		else return this->make_token(Token::LT, "<");
	}
	if (c == '>') {
		c = peak_char();
		if (c == '=') {pop_char(); return this->make_token(Token::GE, ">=");}
		else return this->make_token(Token::GT, ">");
	}
	// type operators
	if (c == '?') return this->make_token(Token::OPT, "?");
	if (c == '^') return this->make_token(Token::PTR, "^");
	if (c == '&') return this->make_token(Token::REF, "&");
	// symbols
	if (c == ':') {
		if (peak_char() == '=') {pop_char(); return this->make_token(Token::ASSIGN, ":=");}
		else return this->make_token(Token::COLON, ":");
	}
	if (c == ';') return this->make_token(Token::SEMICOLON, ";");
	if (c == ',') return this->make_token(Token::COMMA, ",");
	if (c == '.') return this->make_token(Token::DOT, ".");
	if (c == '|') return this->make_token(Token::PIPE, "|");
	// whitespace
	if (c == '\n') {
		unsigned int count = 1;
		while (peak_char() == '\n') pop_char(), count++;
		unsigned int indent = 0;
		while (peak_char() == '\t') pop_char(), indent++;
		indent_diff = indent - line_indent;
		line_indent = indent;
		at_start_of_line = true;
		return this->make_token(Token::NL, std::string(count, '\n'));
	}
	if (c == ' ') {
		while (peak_char() == ' ') pop_char();
		return next_token();
	}
	if (c == '\0') return this->make_token(Token::ENDINPUT, "\0");

	return this->make_token(Token::ERROR, std::string(1, c));
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
	{Token::Type::THROUGH,   std::regex("~>")},
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
	this->column++;
	if (c == '\n') {
		this->column = 0;
		this->line++;
	}
	return c;
}
