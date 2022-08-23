#include "lexer.h"

int main() {
	Lexer lexer("let test_1 42\"Tesing \\\"strings\\\"\"()[]{}*/+-=<>< ><=>=->?^&:;.,|:=\n ");
	lexer.debug();
}
