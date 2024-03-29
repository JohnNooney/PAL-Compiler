//
//  Block1Sema.hpp - Semantic Analyser Demo for Block1
//  CompilerKit
//
//  Created by Amy Parent on 26/11/2021.
//
#include "Block1Sema.hpp"

Block1Sema::Block1Sema(Parser& parser) : Sema(parser) {
}

Block1Sema::~Block1Sema() {
}

void Block1Sema::define(const Token& var, Type type) {
    if(Scope::current()->isDefined(var.text())) {
        redeclaredError(var);
        return;
    }
    Scope::current()->define(var, type);
}

Type Block1Sema::checkVariable(const Token& var) {
    Type type = variableType(var);
    if(type == Type::Invalid) {
        undeclaredError(var);
    }
    return type;
}

Type Block1Sema::variableType(const Token& var) {
    const Symbol* sym = Scope::current()->get(var.text());
    return sym ? sym->type : Type::Invalid;
}

Type Block1Sema::checkExpression(Type lhs, const Token& op, Type rhs) {
    // If both sides of the expression are already invalid, we can just exit early.
    if(lhs == Type::Invalid && rhs == Type::Invalid) return Type::Invalid;
    
    // If only one side of the expression is of invalid type, we don't want to throw an
    // error again. Instead, we assume the user meant it should be the same type as the other
    // side.
    if(lhs == Type::Invalid) lhs = rhs;
    if(rhs == Type::Invalid) rhs = lhs;
    
    // Throw an error if both side are of different types.
    if(lhs != rhs) {
        binaryExprError(op, lhs, rhs);
        return Type::Invalid;
    }
    return lhs;
}

void Block1Sema::checkBoolean(Type lhs, const Token& op, Type rhs) {
    // If both sides of the expression are already invalid, we can just exit early.
    if(lhs == Type::Invalid && rhs == Type::Invalid) return;
    
    // If only one side of the expression is of invalid type, we don't want to throw an
    // error again. Instead, we assume the user meant it should be the same type as the other
    // side.
    if(lhs == Type::Invalid) lhs = rhs;
    if(rhs == Type::Invalid) rhs = lhs;
    
    // Throw an error if both side are of different types.
    if(lhs != rhs) {
        booleanExprError(op, lhs, rhs);
        return;
    }
    return;
}

void Block1Sema::checkAssign(const Token& var, Type rhs) {
    
    // Check that the variable we're assigning to is declared before we do anything else.
    if(!Scope::current()->isDefined(var.text())) {
        undeclaredError(var);
        return;
    }
    
    Type lhs = variableType(var);
    
    // If the expression is of invalid type, we don't want to throw an error again. Instead, we 
    // assume the user meant it to be the variable's type.
    if(rhs == Type::Invalid) rhs = lhs;
    
    // Throw an error if both side are of different types.
    if(lhs != rhs) {
        assignmentError(var, lhs, rhs);
    }
}
