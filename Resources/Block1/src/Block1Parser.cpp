//
//  Block1Parser.cpp - Full-fledged compiler driver demo for the Block1 Language
//  CompilerKit
//
//  Created by Amy Parent on 09/11/2021.
//
#include "Block1Parser.hpp"
#include "CompilerKit/Sema.hpp"

bool Block1Parser::compile() {
    scanner().lex();
    
    Scope::open();
    recDeclarations();
    recBlock();
    Scope::close();
    
    return errors().size() == 0;
}

// <Declarations> ::= <Declaration> <Declarations> | <> ;
// <Declaration> ::= <Type> Identifier <Identifier-List> ;
void Block1Parser::recDeclarations() {
    while(have("int") || have("real")) {
        
        Type type = Type::Invalid;
        if(match("int")) {
            type = Type::Integer;
        } else if(match("real")) {
            type = Type::Real;
        }
        
        do {
            Token var = current();
            if(expect(Token::Identifier)) {
                // Only declare variables in Sema if they're actually valid syntactically.
                sema.define(var, type);
            }
        } while(match(","));
    }
}

// <Block> ::= <Statement> |
//             begin <Statement-List> end ;
void Block1Parser::recBlock() {
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
void Block1Parser::recStatement() {
    if(have("let")) {
        recLet();
    } else if(have("for")) {
        recFor();
    } else if(match("get")) {
        Token var = current();
        if(expect(Token::Identifier)) {
            sema.checkVariable(var);
        }
    } else if(match("put")) {
        recExpression();
    } else {
        syntaxError("<statement>");
    }
}

// <Statement-List> ::= <Statement> <Statement-List> | <> ;
void Block1Parser::recStatementList() {
    if(have("let") || have("for") || have("get") || have("put")) {
        recStatement();
        recStatementList();
    } else {}
}

// <Let-Statement> ::= let Identifier := <Expression> ;
void Block1Parser::recLet() {
    expect("let");
    
    Token var = current();
    expect(Token::Identifier);
    expect(":=");
    Type rhs = recExpression();
    
    sema.checkAssign(var, rhs);
}

// <For-Statement> ::= for Identifier := <Expression> to <Expression> do <Block> ;
void Block1Parser::recFor() {
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
Type Block1Parser::recExpression() {
    auto type = recTerm();
    Token op = current();
    
    // We could also do this recursively -- try and do it that way too!
    while(match("+") || match("-")) {
        auto rhs = recTerm();
        type = sema.checkExpression(type, op, rhs);
        op = current();
    }
    return type;
}

// <Term> ::= <Factor> <Rest-Term> ;
// <Rest-Term> ::= * <Factor> <Rest-Term> |
//                 / <Factor> <Rest-Term> |
//  
Type Block1Parser::recTerm() {
    auto type = recFactor();
    Token op = current();
    while(match("*") || match("/")) {
        auto rhs = recFactor();
        type = sema.checkExpression(type, op, rhs);
        op = current();
    }
    return type;
}

// <Factor> ::= Identifier | IntValue | RealValue | "(" <Expression> ")" ;
Type Block1Parser::recFactor() {
    if(have(Token::Identifier)) {
        Token var = current();
        expect(Token::Identifier);
        return sema.checkVariable(var);
    } else if(match(Token::Integer)) {
        return Type::Integer;
    } else if(match(Token::Real)) {
        return Type::Real;
    } else if(match("(")) {
        auto type = recExpression();
        expect(")");
        return type;
    } else {
        syntaxError("<factor>");
        return Type::Invalid;
    }
}
