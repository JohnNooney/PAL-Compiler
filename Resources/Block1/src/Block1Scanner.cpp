//
//  Block1Scanner.cpp
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
#include "Block1Scanner.hpp"
#include <sstream>
#include <string>
#include <set>

Token Block1Scanner::lex() {
    
    // The list of Block1's keywords
    static const std::set<std::string> keywords = {
        "begin", "end", "int", "real", "let",
        "for", "to", "do", "get", "put"
    };
    
    // Enum that lets us keep track of our state machine
    enum class State {
        Start           = 0,
        Identifier      = 1,
        Integer         = 2,
        Real            = 3,
        Colon           = 4,
        Equals          = 5,
        Punct           = 6,
        EndOfFile       = 98,
        InvalidChar     = 99,
    };
    
    State state = State::Start;
    std::stringstream buf;
    
    while(true) {
        switch(state) {
            case State::Start:
                if(std::isspace(peek())) {
                    state = State::Start;
                } else {
                    updateTokenStart(); // This ensures we don't start the token on whitespace
                    if(std::isalpha(peek())) state = State::Identifier;
                    else if(std::isdigit(peek())) state = State::Integer;
                    else if(peek() == '\0') state = State::EndOfFile;
                    else if(peek() == ':') state = State::Colon;
                    else if(std::strchr("+-*/(),", peek()) != nullptr) state = State::Punct;
                    else state = State::InvalidChar;
                }
                break;
                
            case State::Identifier:
                if(std::isalnum(peek())) {
                    state = State::Identifier;
                } else {
                    // We have an identifier, but we must check first if it is a keyword or not
                    std::string str = buf.str();
                    if(keywords.find(str) != keywords.end()) {
                        return makeToken(str);
                    } else {
                        return makeToken(Token::Identifier, str);
                    }
                }
                break;
                
            case State::Integer:
                if(std::isdigit(peek())) {
                    state = State::Integer;
                } else if(peek() == '.') {
                    state = State::Real;
                } else {
                    return makeToken(Token::Integer, buf.str());
                }
                break;
                
            case State::Real:
                if(std::isdigit(peek())) {
                    state = State::Real;
                } else {
                    return makeToken(Token::Real, buf.str());
                }
                break;
                
            case State::Colon:
                if(peek() == '=') state = State::Equals;
                else return makeToken(Token::InvalidToken, buf.str());
                break;
                
            case State::Equals:
                return makeToken(":=");
                
            case State::Punct:
                return makeToken(buf.str());
                
            case State::EndOfFile:
                return makeToken(Token::EndOfFile);
                
            case State::InvalidChar:
                return makeToken(Token::InvalidChar);
        }
        
        if(state != State::Start) {
            // We are parsing a token, so we keep adding to our token string's buffer.
            buf << peek();
        }
        advance();
    }
    return makeToken(Token::InvalidToken);
}