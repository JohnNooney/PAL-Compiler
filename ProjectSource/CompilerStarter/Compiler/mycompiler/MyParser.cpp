#include "MyParser.hpp"

void MyParser::recStarter() {
	scanner().lex();

	recDeclarations();
	recBlock();
}

// This is based off the BNF for declaration
// <Declaration> ::= (int | real) Identifier ( , Identifier)* ;
void MyParser::recDeclarations()
{
	if (match("int") || match("real")) {
		do {
			expect(Token::Identifier);
		} while (match(","));
	}
}

// Based off the BNF of Block
// <Block> ::= <Statement> | begin <Statement-List> end ;
void MyParser::recBlock()
{
	if (match("begin")) {
		recStatementList();
		expect("end");
	}
	else {
		recStatement();
	}
}

// Based off the BNF for Statement
// <Statement> ::= <Let-Statement> | 
//				   <For-Statement> |
//				   get Identifier |
//				   put <Expession> ;
void MyParser::recStatement()
{
	// Check if either for/let/get/put
	if (have("let")){
		recLet();
	}
	else if (have("for")) {
		recFor();
	}
	else if (match("get")) {
		expect(Token::Identifier);
	}
	else if (match("put")) {
		expect(Token::Identifier);
	}
	else {
		syntaxError("<statement>");
	}
}

//Will parse all statements that exist
// <Statement-List> ::= <Statement> <Statement-List> | <> ;
void MyParser::recStatementList()
{
	if (have("let") || have("for") || have("get") || have("put")) {
		recStatement();
		recStatementList();
	}else {}
}

//Based off the BNF 
// <For-Statement> ::= for Identifier := <Expression> to <Expression> do <Block> ;
void MyParser::recFor() {
	expect("for");
	expect(Token::Identifier);
	expect(":=");
	recExpression();
	expect("to");
	recExpression();
	expect("do");
	recBlock();
}

//Based off the BNF
// <Let-Statememt> ::= let Identifier := <Expression> ;
void MyParser::recLet() {
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
void MyParser::recExpression() {
	recTerm();
	while (match("+") || match("-")) {
		recExpression();
	}
}

//Base off the BND 
// <Term> ::= <Factor> <Rest-Term> ;
// and
// <Rest-Term> ::= * <Factor> <Rest-Term> | 
//				   / <Facor> <Rest-Term> |
//				   <>
void MyParser::recTerm()
{
	recFactor();
	while (match("*") || match("/")) {
		recTerm();
	}
}

//Based off the BNF
//
void MyParser::recFactor()
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

