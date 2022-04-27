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
		//recIO();
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

	Symbol right = recExpression();

	//compare types of variable and expression result
	sema.checkAssign(var, right.type);
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
	// TODO: Make sure to only run this code when bool is true
	/*if (result) {*/
		while (statementCheck()) {
			recStatement();
		}
	//}
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
bool MyPALParser::recBooleanExpr()
{
	auto left = recExpression();

	Token op = current();
	if (match(">") || match("<") || match("=")) {
		auto right = recExpression();
		sema.checkBoolean(left.type, op, right.type);

		// perform comparison
		if (op.text() == ">") {
			// TODO: store operator for a less than comparison
			std::cout << "num value: " << left.token.value<int>() << std::endl;
			std::cout << "num2 value: " << right.token.value<float>() << std::endl;
			return left.token.value<float>() > right.token.value<float>();
		}
		else if (op.text() == "<") {
			// TODO: store operator for a greater than comparison
		}
		else if (op.text() == "=") {
			// TODO: store operator for an equal comparison
		}
	}
	else {
		syntaxError("<BooleanExpr>");
	}

	return false;
}

//// Based off the EBNF - Input = read from standard input | Output = write to standard output
//// <I-o> ::= INPUT <IdentList> | OUTPUT <Expression>(, <Expression>)*;
//void MyPALPureParser::recIO()
//{
//	if (match("INPUT")) {
//		recIdentList();
//	}
//	else if (match("OUTPUT")) {
//		do {
//			recExpression();
//		} while (match(","));
//	}
//	else {
//		syntaxError("<I-o>");
//	}
//
//}

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
Symbol MyPALParser::recExpression()
{
	auto left =	recTerm();
	Token op = current();

	while (match("+") || match("-")) {
		auto right = recTerm();
		left.type = sema.checkExpression(left.type, op, right.type);
		op = current();
	}

	return left;
}

// Based off the EBNF
// <Term> ::= <Factor> ( (*|/) <Factor>)* ;
Symbol MyPALParser::recTerm()
{
	auto left = recFactor();
	Token op = current();

	while (match("*") || match("/")) {
		auto right = recFactor();
		left.type = sema.checkExpression(left.type, op, right.type);
		op = current();
	}

	// TODO: perform the computation?
	return left;
}

// Based off the EBNF
// <Factor> ::= (+|-)? ( <Value> | "(" <Expression> ")" ) ;
// and
// <Value> ::= Identifier | IntegerValue | RealValue ;
Symbol MyPALParser::recFactor()
{
	if (match("+") || match("-")) {
		// TODO: store token

	}

	if (match("(")) {
		auto type = recExpression();
		expect(")");
		return type;
	}
	else if (have(Token::Identifier)) {
		Token var = current();
		expect(Token::Identifier);
		Symbol tempSymb = Symbol{ var, sema.checkVariable(var) };
		return tempSymb;
	}
	else if (match(Token::Integer)) {
		Token var = current();
		Symbol tempSymb = Symbol{var, Type::Integer};
		return tempSymb;
	}
	else if (match(Token::Real)) {
		Token var = current();
		Symbol tempSymb = Symbol{ var, Type::Real };
		return tempSymb;
	}
	else {
		syntaxError("<Factor>");
		Token var = current();
		Symbol tempSymb = Symbol{ var, Type::Invalid };
		return tempSymb;
	}
}


