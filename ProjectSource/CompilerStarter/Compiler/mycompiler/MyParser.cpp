#include "MyParser.hpp"

void MyParser::recStarter() {
	scanner().lex();

	recDeclarations();
	recBlock();
}

// This is based off the EBNF for declaration
// <Declaration> ::= (int | real) Identifier ( , Identifier)* ;
void MyParser::recDeclarations()
{
	if (match("int") || match("real")) {
		do {
			expect(Token::Identifier);
		} while (match(","));
	}
}

void MyParser::recBlock()
{
}
