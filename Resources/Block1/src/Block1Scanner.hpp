//
//  Block1Scanner.hpp
//
//  Created by Amy Parent on 24/11/2021.
//  Based on work by Allan Milne.
//
//  Original work:
//
//  File: Block1Scanner.cs
//  Purpose: A scanner for the Block-1 language.
//  Version: 4.1
//  Date: 13th April 2011.
//  Author: Allan C. Milne.
//
//  Namespace: AllanMilne.Block1
//  Requires: Ardkit.dll
//  Exposes: Block1Scanner.
//
//  Description:
//  Uses version 2 of the Ardkit toolkit and reflects EBNF version 3.
//  The Block-1 language is defined in the file Block-1.BNF.txt.
//  A finite state machine (FSM) is used to implement the getNextToken() method that
//  realises the scanning operation;
//  a state transition pattern is used to implement the FSM.
//  the FSM can be found in Block-1.FSM.txt.
#include "CompilerKit/Scanner.hpp"

using namespace CompilerKit;

class Block1Scanner : public Scanner {
public:
    Block1Scanner(std::istream& in) : Scanner(in) {}
    virtual ~Block1Scanner() {}
    
    Token lex() override;
private:

};
