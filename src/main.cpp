#include "lexer.h"
#include "parser.h"

int main() {
	//Lexer lexer("let test_1 42\"Tesing \\\"strings\\\"\"()[]{}*/+-=<>< ><=>=->?^&:;.,|:=\n ");
	Lexer lexer(
		"let x = 42"
		"let y = 2"
	);

	lexer.debug();
	lexer.reset();

	Parser parser(lexer);
	parser.parse()->print(std::cout);
}
