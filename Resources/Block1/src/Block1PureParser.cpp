//
//  Block1PureParser.cpp - Full-fledged compiler driver demo for the Block1 Language
//  CompilerKit
//
//  Created by Amy Parent on 09/11/2021.
//
#include "Block1PureParser.hpp"

void Block1PureParser::compile() {
    scanner().lex();
    
    recDeclarations();
    recBlock();
}

// <Declarations> ::= <Declaration> <Declarations> | <> ;
// <Declaration> ::= <Type> Identifier <Identifier-List> ;
void Block1PureParser::recDeclarations() {
    while(match("int") || match("real")) {
        do {
            expect(Token::Identifier);
        } while(match(","));
    }
}

// <Block> ::= <Statement> |
//             begin <Statement-List> end ;
void Block1PureParser::recBlock() {
    if(match("begin")) {
        recStatementList();
        expect("end");
    } else {
        recStatement();
    }
}

// <Statement> ::= <Let-Statement>  |
//                 <For-Statement>  |
//                 get Identifier   |
//                 put <Expression> ;
void Block1PureParser::recStatement() {
    if(have("let")) {
        recLet();
    } else if(have("for")) {
        recFor();
    } else if(match("get")) {
        expect(Token::Identifier);
    } else if(match("put")) {
        recExpression();
    } else {
        syntaxError("<statement>");
    }
}

// <Statement-List> ::= <Statement> <Statement-List> | <> ;
void Block1PureParser::recStatementList() {
    if(have("let") || have("for") || have("get") || have("put")) {
        recStatement();
        recStatementList();
    } else {}
}

// <Let-Statement> ::= let Identifier := <Expression> ;
void Block1PureParser::recLet() {
    expect("let");
    expect(Token::Identifier);
    expect(":=");
    recExpression();
}

// <For-Statement> ::= for Identifier := <Expression> to <Expression> do <Block> ;
void Block1PureParser::recFor() {
    expect("for");
    expect(Token::Identifier);
    expect(":=");
    recExpression();
    expect("to");
    recExpression();
    expect("do");
    recBlock();
}

// <Expression> ::= <Term> <Rest-Expr> ;
// <Rest-Expr> ::= "+" <Term> <Rest-Expr> |
//                 - <Term> <Rest-Expr>   |
//                 <> ;
void Block1PureParser::recExpression() {
    recTerm();
    
    // We could also do this recursively -- try and do it that way too!
    while(match("+") || match("-")) {
        recTerm();
    }
}

// <Term> ::= <Factor> <Rest-Term> ;
// <Rest-Term> ::= * <Factor> <Rest-Term> |
//                 / <Factor> <Rest-Term> |
//                   <> ;
void Block1PureParser::recTerm() {
    recFactor();
    while(match("*") || match("/")) {
        recFactor();
    }
}

// <Factor> ::= Identifier | IntValue | RealValue | "(" <Expression> ")" ;
void Block1PureParser::recFactor() {
    if(match(Token::Identifier)) {
        // We have a variable reference
    } else if(match(Token::Integer)) {
        
    } else if(match(Token::Real)) {
        
    } else if(match("(")) {
        recExpression();
        expect(")");
    } else {
        syntaxError("<factor>");
    }
}
