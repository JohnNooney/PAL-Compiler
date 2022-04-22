#ifndef _MyPALParser_HPP_
#define _MyPALParser_HPP_

#include "../CompilerKit/Parser.hpp"
#include "MyPALSema.hpp"
#include "list"

using namespace CompilerKit;

class MyPALParser : public RecoveringParser {
public:

    MyPALParser(Scanner& scanner) : RecoveringParser(scanner), sema(*this) {}
    virtual ~MyPALParser() {}

    bool compile();
    void recStarter();

private:
    void recVarDecls();
    void recStatement();
    bool statementCheck();
    /*void recAssignment();
    void recLoop();
    void recConditional();
    void recIO();

    Type recExpression();
    Type recTerm();
    Type recFactor();
    bool recBooleanExpr();*/
    std::list<Token> recIdentList();

    MyPALSema sema;
};

#endif 