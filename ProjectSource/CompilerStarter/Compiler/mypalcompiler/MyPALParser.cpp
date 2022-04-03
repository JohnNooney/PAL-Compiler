#include "MyPALParser.hpp"

// Based off the program EBNF
// <Program> ::= PROGRAM Identifier
/*				WITH <VarDecls>
				IN(<Statement>) +
				END;
*/
void MyPALParser::recStarter() {
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
	expect(";");
}

bool MyPALParser::statementCheck() {
	return have(Token::Identifier) || have("UNTIL") || have("IF") || have("INPUT") ? true : false;
}

// Based off the EBNF
// <VarDecls> ::= (<IdentList> AS <Type>)* ;
void MyPALParser::recVarDecls() {
	while(have(Token::Identifier)) {
		recIdentList();
		expect("AS");

		// TODO: refactor to use sema to register token
		if (match("REAL") || match("INTEGER"))
		{
		} 
		else {
			// TODO: throw error missing type
		}
	}
}

// Based off the EBNF 
// <Statement> ::= <Assignment> | <Loop> | <Conditional> | <I-o> ;
void MyPALParser::recStatement()
{
	if (have(Token::Identifier)) {
		// recognise Assignment
	}
	else if (have("UNTIL")) {
		// recognise Loop
	}
	else if (have("IF")) {
		// recognise Conditional
	}
	else if (have("INPUT")) {
		// recognise IO
	}
	else {
		// Throw error
	}
}

// Based off the EBNF 
// <Assignment> ::= Identifier = <Expression> ;
void MyPALParser::recAssignment()
{
	expect(Token::Identifier);
	expect("=");
	recExpression();
}

// Based off the EBNF
// <Conditional> ::= IF <BooleanExpr> THEN (<Statement>)*
//						(ELSE(<Statement>)*)?
//						ENDIF;
void MyPALParser::recConditional()
{
	expect("IF");
	recBooleanExpr();
	expect("THEN");
	// TODO: Make sure to only run this code when bool is true
	while (statementCheck()) {
		recStatement();
	}
	if (match("ELSE")) {
		while (statementCheck()) {
			recStatement();
		}
	}
	expect("ENDIF");
	expect(";");
}

// Based off the EBNF - returns bool based on the result
// <BooleanExpr> ::= <Expression> ("<" | "=" | ">") <Expression> ;
bool MyPALParser::recBooleanExpr()
{
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
	}

	recExpression();

	// TODO: perform comparison
	return true;
}

// Based off the EBNF - Input = read from standard input | Output = write to standard output
// <I-o> ::= INPUT <IdentList> | OUTPUT <Expression>(, <Expression>)*;
void MyPALParser::recIO()
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
		// TODO: Throw IO error
	}
	
}

// Based off the EBNF 
// <IdentList> ::= Identifier ( , Identifier)* ;
void MyPALParser::recIdentList()
{
	do {
		expect(Token::Identifier);
	} while (match(","));
}

// Based off the EBNF - should return the type
// <Expression> ::= <Term> ( (+|-) <Term>)* ;
void MyPALParser::recExpression()
{
	recTerm();
	while (match("+") || match("-")) {
		recTerm();
	}
}

// Based off the EBNF
// <Term> ::= <Factor> ( (*|/) <Factor>)* ;
void MyPALParser::recTerm()
{
	recFactor();
	while (match("*") || match("/")) {
		recFactor();
	}
}

// Based off the EBNF
// <Factor> ::= (+|-)? ( <Value> | "(" <Expression> ")" ) ;
//void MyPALParser::recFactor()
{
	if (match("+") || match("-")) {
		// TODO: store token
	}

	if (match("(")) {
		recExpression();
		expect(")");
		expect(";");
	}
	else {
		recValue();
	}


}

//void MyPALParser::recValue()
//{
//}
//

