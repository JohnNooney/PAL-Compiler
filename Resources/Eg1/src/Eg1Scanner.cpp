#include "Eg1Scanner.hpp"
#include <cctype>
#include <sstream>

// find and return the next token using a state transition FSM.
Token Eg1Scanner::lex() {
    
    int state = 0;
    std::stringstream buf;

    while(true) {
        switch(state) {
        case 0:     // start state.
            if(std::isspace(peek())) {
                state = 0;
            } else {
                updateTokenStart();
                if(std::isalpha(peek()))        state = 1;
                else if(std::isdigit(peek()))   state = 2;
                else if(peek() == '=')          state = 3;
                else if(peek() == '>')          state = 4;
                else if(peek() == '\0')         state = 6;
                else                            state = 7;
            }
            break;
            
        case 1:     // Identifier
            if(std::isalnum(peek())) {
                state = 1;
            } else {
                return makeToken(Token::Identifier, buf.str());
            }
            break;
            
        case 2:     // Integer
            if(std::isdigit(peek())) {
                state = 2;
            } else {
                return makeToken(Token::Integer, buf.str());
            }
            break;
            
        case 3:
            return makeToken("=");
            break;
            
        case 4:     // > or >=
            if(peek() == '=') {
                state = 5;
            } else {
                return makeToken(">");
            }
            break;
            
        case 5:
            return makeToken(">=");
            
        case 6:     // end-of-file
            return makeToken(Token::EndOfFile);
            
        case 7:     // invalid-char
            return makeToken(Token::InvalidChar, buf.str());
        }
        
        if(state != 0) {
            // We are parsing a token, so we keep adding to our token string's buffer.
            buf << peek();
        }
        advance();
    }
}
