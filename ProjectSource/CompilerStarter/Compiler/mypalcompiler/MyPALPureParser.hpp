#ifndef _MyPALParser_HPP_
#define _MyPALParser_HPP_

#include "../CompilerKit/Parser.hpp"
using namespace CompilerKit;

class MyPALPureParser: public RecoveringParser {
public:

    MyPALPureParser(Scanner& scanner) : RecoveringParser(scanner) {}
    virtual ~MyPALPureParser() {}

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
    bool recBooleanExpr();
    void recIdentList();


};

#endif 