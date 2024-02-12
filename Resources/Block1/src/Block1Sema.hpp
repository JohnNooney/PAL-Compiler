//
//  Block1Sema.hpp - Semantic Analyser Demo for Block1
//  CompilerKit
//
//  Created by Amy Parent on 26/11/2021.
//
#ifndef _BLOCK1SEMA_HPP_
#define _BLOCK1SEMA_HPP_

#include "CompilerKit/Sema.hpp"
using namespace CompilerKit;

class Block1Sema : public Sema {
public:
    Block1Sema(Parser& parser);
    virtual ~Block1Sema();
    
    void define(const Token& var, Type type);
    
    Type checkVariable(const Token& var);
    Type variableType(const Token& var);
    
    Type checkExpression(Type lhs, const Token& op, Type rhs);
    void checkBoolean(Type lhs, const Token& op, Type rhs);
    void checkAssign(const Token& var, Type rhs);
};

#endif /* ifndef _BLOCK1SEMA_HPP_ */
