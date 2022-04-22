#include "MyPALPureParser.hpp"

// Based off the program EBNF
// <Program> ::= PROGRAM Identifier
/*				WITH <VarDecls>
				IN(<Statement>) +
				END;
*/
void MyPALPureParser::recStarter() {
	scanner().lex();

	// start parsing
	expect("PROGRAM");
	expect(Token::Identifier);
	expect("WITH");
	recVarDecls();
	expect("IN");
	do {
		recStatement();
	} while (statementCheck());
	expect("END");
}

bool MyPALPureParser::statementCheck() {
	return have(Token::Identifier) || have("UNTIL") || have("IF") || have("INPUT") || have("OUTPUT") ? true : false;
}

// Based off the EBNF
// <VarDecls> ::= (<IdentList> AS <Type>)* ;
void MyPALPureParser::recVarDecls() {
	while(have(Token::Identifier)) {
		recIdentList();
		expect("AS");

		// TODO: refactor to use sema to register token
		if (match("REAL") || match("INTEGER"))
		{
		} 
		else {
			// TODO: throw error missing type
			syntaxError("<VarDecls>");
		}
	}
}

// Based off the EBNF 
// <Statement> ::= <Assignment> | <Loop> | <Conditional> | <I-o> ;
void MyPALPureParser::recStatement()
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
void MyPALPureParser::recAssignment()
{
	expect(Token::Identifier);
	expect("=");
	recExpression();
}

// Based off the EBNF
// If <BooleanExpr> is true, skip to the statement following the
// ENDLOOP, otherwise execute the statements and repeat.
// <Loop> :: = UNTIL <BooleanExpr> REPEAT(<Statement>) * ENDLOOP;
void MyPALPureParser::recLoop()
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
void MyPALPureParser::recConditional()
{
	expect("IF");
	bool result = recBooleanExpr();
	expect("THEN");
	// TODO: Make sure to only run this code when bool is true
	if (result) {
		while (statementCheck()) {
			recStatement();
		}
	}
	// else if (!return && match("ELSE")) this will not work because it hasn't been parsed
	if (match("ELSE")) {
		while (statementCheck()) {
			// TODO: pass !result if it should be executed
			recStatement();
		}
	}
	expect("ENDIF");
}

// Based off the EBNF - returns bool based on the result
// <BooleanExpr> ::= <Expression> ("<" | "=" | ">") <Expression> ;
bool MyPALPureParser::recBooleanExpr()
{	
	//TODO: store as left
	recExpression();

	if (match("<")) {
		// TODO: store operator for a less than comparison
	}
	else if (match(">")) {
		// TODO: store operator for a greater than comparison
	}
	else if (match("=")) {
		// TODO: store operator for an equal comparison
	}
	else {
		// TODO: Throw Error
		syntaxError("<BooleanExpr>");
	}

	//TODO: store as right
	recExpression();

	// TODO: perform comparison
	return true;
}

// Based off the EBNF - Input = read from standard input | Output = write to standard output
// <I-o> ::= INPUT <IdentList> | OUTPUT <Expression>(, <Expression>)*;
void MyPALPureParser::recIO()
{
	if (match("INPUT")) {
		recIdentList();
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
void MyPALPureParser::recIdentList()
{
	do {
		expect(Token::Identifier);
	} while (match(","));
}

// Based off the EBNF - should return the type
// <Expression> ::= <Term> ( (+|-) <Term>)* ;
void MyPALPureParser::recExpression()
{
	recTerm();
	while (match("+") || match("-")) {
		recTerm();
	}
}

// Based off the EBNF
// <Term> ::= <Factor> ( (*|/) <Factor>)* ;
void MyPALPureParser::recTerm()
{
	recFactor();
	while (match("*") || match("/")) {
		recFactor();
	}
}

// Based off the EBNF
// <Factor> ::= (+|-)? ( <Value> | "(" <Expression> ")" ) ;
// and
// <Value> ::= Identifier | IntegerValue | RealValue ;
void MyPALPureParser::recFactor()
{
	if (match("+") || match("-")) {
		// TODO: store token
	}

	if (match("(")) {
		recExpression();
		expect(")");
	}
	else if (match(Token::Identifier)) {
		
	}
	else if (match(Token::Integer)) {

	}
	else if (match(Token::Real)) {

	}
	else {
		syntaxError("<Factor>");
	}
}

