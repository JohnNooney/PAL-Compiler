#ifndef _MYPALSEMA_HPP_
#define _MYPALSEMA_HPP_

#include "../CompilerKit/Sema.hpp"
using namespace CompilerKit;

class MyPALSema : public Sema {
public:
    MyPALSema(Parser& parser);
    virtual ~MyPALSema();

    void define(const Token& var, Type type);

    Type checkVariable(const Token& var);
    Type variableType(const Token& var);

    Type checkExpression(Type left, const Token& op, Type right);
    void checkBoolean(Type left, const Token& op, Type right);
    void checkAssign(const Token& var, Type right);
};

#endif 