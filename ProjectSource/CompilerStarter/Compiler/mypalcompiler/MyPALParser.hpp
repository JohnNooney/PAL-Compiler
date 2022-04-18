#ifndef _MyPALParser_HPP_
#define _MyPALParser_HPP_

#include "../CompilerKit/Parser.hpp"
using namespace CompilerKit;

class MyPALParser: public Parser {
public:

    MyPALParser(Scanner& scanner) : Parser(scanner) {}
    virtual ~MyPALParser() {}

    bool compile();
    void recStarter();

private:
    void recVarDecls();
    void recStatement();
    bool statementCheck();
    void recAssignment();
    void recLoop();
    void recConditional();
    void recIO();
    void recExpression();
    void recTerm();
    void recFactor();
    void recValue();
    bool recBooleanExpr();
    void recIdentList();


};

#endif 