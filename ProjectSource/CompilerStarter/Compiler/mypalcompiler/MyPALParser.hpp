#ifndef _MyPALParser_HPP_
#define _MyPALParser_HPP_

#include "../CompilerKit/Parser.hpp"
using namespace CompilerKit;

class MyPALParser: public RecoveringParser {
public:

    MyPALParser(Scanner& scanner) : RecoveringParser(scanner) {}
    virtual ~MyPALParser() {}

    bool compile();
    void recStarter();

private:
    void recVarDecls();
    void recStatement();
    void recAssignment();
    void recLoop();
    void recConditional();
    void recIO();
    void recExpression();
    void recTerm();
    void recFactor();
    void recValue();
    void recBooleanExpr();
    void recIdentList();


};

#endif 