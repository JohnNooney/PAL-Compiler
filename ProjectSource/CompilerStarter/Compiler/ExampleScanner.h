// ExampleScanner.h
//
// An extremely basic scanner that subclasses CompilerKit's Scanner class. `ExampleScanner` does nothing
// but return invalid tokens, but you can use it as a skeletton for your actual scanners -- it has all
// the boilerplate you need to start
//
#pragma once
#include "CompilerKit/Scanner.hpp"

// This line is optional, it only allows us to type "Scanner" instead of "CompilerKit::Scanner" every time.
using namespace CompilerKit;


// Our Scanner subclass. Remember to make the inheritance public!
class ExampleScanner : public Scanner {
public:

	// The constructor does nothing but call the superclass's constructor for now.
	// If your scanner has internal data, you can initialise it here
	ExampleScanner(std::istream& source) : Scanner(source) {
		// <your initialisation code>
	}

	// The main lexing function! this is where you'll implement your FSM to actually
	// lex tokens for your language
	Token lex() {
		return makeToken(Token::InvalidToken);
	}

private:
	// <your scanner's internal data>
};

