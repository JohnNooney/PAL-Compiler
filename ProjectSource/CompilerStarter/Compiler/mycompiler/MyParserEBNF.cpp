#include "MyParserEBNF.hpp"

void MyParserEBNF::recStarter() {
	scanner().lex();

	recDeclaration();
	recBlock();
}

// This is based off the EBNF for declaration
// <Declaration> ::= (int | real) Identifier ( , Identifier)* ;
void MyParserEBNF::recDeclaration()
{
	if (match("int") || match("real")) {
		do {
			expect(Token::Identifier);
		} while (match(","));
	}
}

// Based off the EBNF of Block
// <Block> ::= <Statement> | begin <Statement-List> end ;
void MyParserEBNF::recBlock()
{
	if (have("let") || have("for") || have("get") || have("put")) {
		recStatement();
	}
	else {
		expect("begin");
		while (have("let") || have("for") || have("get") || have("put")) {
			recStatement();
		}
		expect("end");
	}
	
}

// Based off the EBNF for Statement
// <Statement> ::= <Let-Statement> | 
//				   <For-Statement> |
//				   get Identifier |
//				   put <Expession> ;
void MyParserEBNF::recStatement()
{
	// Check if either for/let/get/put
	if (have("let")) {
		recLet();
	}
	else if (have("for")) {
		recFor();
	}
	else if (match("get")) {
		expect(Token::Identifier);
	}
	else if (match("put")) {
		recExpression();
	}
	else {
		syntaxError("<statement>");
	}
}


//Based off the EBNF 
// <For-Statement> ::= for Identifier := <Expression> to <Expression> do <Block> ;
void MyParserEBNF::recFor() {
	expect("for");
	expect(Token::Identifier);
	expect(":=");
	recExpression();
	expect("to");
	recExpression();
	expect("do");
	recBlock();
}

//Based off the EBNF
// <Let-Statememt> ::= let Identifier := <Expression> ;
void MyParserEBNF::recLet() {
	expect("let");
	expect(Token::Identifier);
	expect(":=");
	recExpression();
}

//Based off the BNF
// <Expression> ::= <Term> <Rest-Expr> ;
// and 
// <Rest-Expr> ::= * <Term> <Rest-Expr> | 
//				   / <Term> <Rest-Expr> |
//				   <>
void MyParserEBNF::recExpression() {
	recTerm();
	while (match("+") || match("-")) {
		recTerm();
	}
}

//Base off the EBNF 
// <Term> ::= <Factor> <Rest-Term> ;
// and
// <Rest-Term> ::= * <Factor> <Rest-Term> | 
//				   / <Facor> <Rest-Term> |
//				   <>
void MyParserEBNF::recTerm()
{
	recFactor();
	while (match("*") || match("/")) {
		recTerm();
	}
}

//Based off the EBNF
void MyParserEBNF::recFactor()
{
	if (match(Token::Identifier)) {

	}
	else if (match(Token::Integer)) {

	}
	else if (match(Token::Real)) {

	}
	else if (match("(")) {
		recExpression();
		expect(")");
	}
	else {
		syntaxError("<factor>");
	}
}

