#include "MyParserEBNF.hpp"
#include "MySema.hpp"

bool MyParserEBNF::compile() {
	scanner().lex();

	Scope::open();
	recStarter();
	Scope::close();

	return errors().size() == 0;
}

void MyParserEBNF::recStarter() {
	recDeclaration();
	recBlock();
}

// This is based off the EBNF for declaration
// <Declaration> ::= (int | real) Identifier ( , Identifier)* ;
void MyParserEBNF::recDeclaration()
{
	//use Sema to store type
	Type type = Type::Invalid;
	if (match("int")) {
		type = Type::Integer;
	} 
	else if (match("real")) {
		type = Type::Real;
	}

	// cycle through each decleration 
	do {
		Token var = current();
		if (expect(Token::Identifier)) {
			// only add to sema if valid
			sema.define(var, type);
		}
		
	} while (match(","));
}

// Based off the EBNF of Block
// <Block> ::= <Statement> | begin(<Statement>)* end;
void MyParserEBNF::recBlock()
{
	if (match("begin")) {
		while (have("let") || have("for") || have("get") || have("put")) {
			recStatement();
		}
		match("end");
	}
	else {
		recStatement();
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
		// return the variable 
		Token var = current();
		if (expect(Token::Identifier)) {
			sema.checkVariable(var);
		}
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

	Token var = current();
	expect(Token::Identifier);
	expect(":=");
	Type right = recExpression();

	// compare types of variable and expression
	sema.checkAssign(var, right);
}

//Based off the EBNF
// <Expression> ::= <Term> ( ("+"|-) <Term>)* ;
Type MyParserEBNF::recExpression() {
	auto type = recTerm();
	Token op = current();

	while (match("+") || match("-")) {
		auto right = recTerm();
		type = sema.checkExpression(type, op, right);
		op = current();
	}

	return type;
}

//Base off the EBNF 
// <Term> ::= <Factor> ( (*|/) <Factor>)* ;
Type MyParserEBNF::recTerm()
{
	auto type = recFactor();
	Token op = current();
	
	while (match("*") || match("/")) {
		auto right = recFactor();
		type = sema.checkExpression(type, op, right);
		op = current();
	}

	return type;
}

//Based off the EBNF
//<Factor> ::= Identifier | IntValue | RealValue | "(" <Expression> ")" ;
Type MyParserEBNF::recFactor()
{
	if (have(Token::Identifier)) {
		Token var = current();
		expect(Token::Identifier);
		return sema.checkVariable(var);
	}
	else if (match(Token::Integer)) {
		return Type::Integer;
	}
	else if (match(Token::Real)) {
		return Type::Real;
	}
	else if (match("(")) {
		auto type = recExpression();
		expect(")");
		return type;
	}
	else {
		syntaxError("<factor>");
		return Type::Invalid;
	}
}

