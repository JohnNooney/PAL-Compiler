#include "MyPALParser.hpp"
#include "MyPALSema.hpp"
#include "list"

bool MyPALParser::compile() {
	scanner().lex();
	
	Scope::open();
	recStarter();
	Scope::close();

	return errors().size() == 0;
}

// Based off the program EBNF
// <Program> ::= PROGRAM Identifier
/*				WITH <VarDecls>
				IN(<Statement>) +
				END;
*/
void MyPALParser::recStarter() {
	

	// start parsing
	expect("PROGRAM");
	expect(Token::Identifier); // do nothing with name of program
	expect("WITH");
	recVarDecls();
	expect("IN");
	do {
		recStatement();
	} while (statementCheck());
	expect("END");
}

bool MyPALParser::statementCheck() {
	return have(Token::Identifier) || have("UNTIL") || have("IF") || have("INPUT") || have("OUTPUT") ? true : false;
}

// Based off the EBNF
// <VarDecls> ::= (<IdentList> AS <Type>)* ;
void MyPALParser::recVarDecls() {
	while (have(Token::Identifier)) {
		std::list<Token> identList = recIdentList();
		expect("AS");

		Type type = Type::Invalid;
		if (match("REAL"))
		{
			type = Type::Real;
		}
		else if (match("INTEGER")) {
			type = Type::Integer;
		}
		else {
			syntaxError("<VarDecls>");
		}

		// add each token to sema
		for (const Token ident : identList)
		{
			sema.define(ident, type);
		}
	}
}

// Based off the EBNF 
// <Statement> ::= <Assignment> | <Loop> | <Conditional> | <I-o> ;
void MyPALParser::recStatement()
{
	if (have(Token::Identifier)) {
		// recognise Assignment
		recAssignment();
	}
	else if (have("UNTIL")) {
		// recognise Loop
		recLoop();
	}
	else if (have("IF")) {
		// recognise Conditional
		recConditional();
	}
	else if (have("INPUT") || have("OUTPUT")) {
		// recognise IO
		recIO();
	}
	else {
		// Throw error
		syntaxError("Statement");
	}
}

// Based off the EBNF 
// <Assignment> ::= Identifier = <Expression> ;
void MyPALParser::recAssignment()
{
	Token var = current();

	expect(Token::Identifier);
	expect("=");

	auto right = recExpression();

	//compare types of variable and expression result
	sema.checkAssign(var, right);
}

// Based off the EBNF
// If <BooleanExpr> is true, skip to the statement following the
// ENDLOOP, otherwise execute the statements and repeat.
// <Loop> :: = UNTIL <BooleanExpr> REPEAT(<Statement>) * ENDLOOP;
void MyPALParser::recLoop()
{
	expect("UNTIL");
	bool result = recBooleanExpr();

	if (result) {
		expect("REPEAT");

		//TODO: still parse but pass false to skip token tracking to end of statement
		while (statementCheck()) {
			recStatement();
		}

		expect("ENDLOOP");
	}
	else {
		expect("REPEAT");

		while (!result)
		{
			// loop through all statements inside
			while (statementCheck()) {
				recStatement();
			}

			result = recBooleanExpr();
		}

		expect("ENDLOOP");
	}

}

// Based off the EBNF
// <Conditional> ::= IF <BooleanExpr> THEN (<Statement>)*
//						(ELSE(<Statement>)*)?
//						ENDIF;
void MyPALParser::recConditional()
{
	expect("IF");
	bool result = recBooleanExpr();
	expect("THEN");
	while (statementCheck()) {
		recStatement();
	}
		
	if (match("ELSE")) {
		while (statementCheck()) {
			recStatement();
		}
	}
	expect("ENDIF");
}

// Based off the EBNF - returns bool based on the result
// <BooleanExpr> ::= <Expression> ("<" | "=" | ">") <Expression> ;
void MyPALParser::recBooleanExpr()
{
	auto left = recExpression();

	Token op = current();
	if (match(">") || match("<") || match("=")) {
		auto right = recExpression();
		sema.checkBoolean(left, op, right);
	}
	else {
		syntaxError("<BooleanExpr> Invalid comparison operator");
	}
}

// Based off the EBNF - Input = read from standard input | Output = write to standard output
// <I-o> ::= INPUT <IdentList> | OUTPUT <Expression>(, <Expression>)*;
void MyPALParser::recIO()
{
	if (match("INPUT")) {
		std::list<Token> identList = recIdentList();

		for (const Token ident : identList)
		{
			// check that each variable exists
			sema.checkVariable(ident);
		}
	}
	else if (match("OUTPUT")) {
		do {
			recExpression();
		} while (match(","));
	}
	else {
		syntaxError("<I-o>");
	}

}

// Based off the EBNF 
// <IdentList> ::= Identifier ( , Identifier)* ;
std::list<Token> MyPALParser::recIdentList()
{
	std::list<Token> identList;

	do {
		Token var = current();

		if (expect(Token::Identifier)) {
			identList.push_back(var);
		}
	} while (match(","));

	return identList;
}

// Based off the EBNF - should return the type
// <Expression> ::= <Term> ( (+|-) <Term>)* ;
Type MyPALParser::recExpression()
{
	auto left =	recTerm();
	Token op = current();

	while (match("+") || match("-")) {
		auto right = recTerm();
		left = sema.checkExpression(left, op, right);
		op = current();


		// TODO: Perform computation
	}

	return left;
}

// Based off the EBNF
// <Term> ::= <Factor> ( (*|/) <Factor>)* ;
Type MyPALParser::recTerm()
{
	auto left = recFactor();
	Token op = current();

	while (match("*") || match("/")) {
		auto right = recFactor();
		left = sema.checkExpression(left, op, right);
		op = current();
	}

	// TODO: perform the computation?
	return left;
}

// Based off the EBNF
// <Factor> ::= (+|-)? ( <Value> | "(" <Expression> ")" ) ;
// and
// <Value> ::= Identifier | IntegerValue | RealValue ;
Type MyPALParser::recFactor()
{
	bool negative = false;
	if (match("+") || match("-")) {
		// TODO: store in token value
		// if - then set value to negative (minus itself x2)
		negative = true;
	}


	if (match("(")) {
		auto type = recExpression();
		expect(")");
		return type;
	}
	else if (have(Token::Identifier)) {
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
	else {
		syntaxError("<Factor>");
		return Type::Invalid;
	}
}


