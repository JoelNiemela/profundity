#include "lexer.h"
#include "parser.h"

#include <fstream>
#include <string>

int main() {
	//Lexer lexer("let test_1 42\"Tesing \\\"strings\\\"\"()[]{}*/+-=<>< ><=>=->?^&:;.,|:=\n ");
	
	std::ifstream file("./test.prfn");
	std::string source((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));

	Lexer lexer(source);

	lexer.debug();
	//lexer.reset();

	//Parser parser(lexer);
	//parser.parse()->print(std::cout);
}
