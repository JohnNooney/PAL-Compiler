#ifndef _MYPARSEREBNF_HPP_
#define _MYPARSEREBNF_HPP_

#include "../CompilerKit/Parser.hpp"
#include "MySema.hpp"
using namespace CompilerKit;


class MyParserEBNF : public RecoveringParser{
public:

    MyParserEBNF(Scanner& scanner) : RecoveringParser(scanner), sema(*this) {}
    virtual ~MyParserEBNF() {}

    bool compile();
    void recStarter();

private:
    void recDeclaration();
    void recStatement();
    void recBlock();
    void recLet();
    void recFor();

    // Return types when variables have been figured out. This pushes the sema to the next step
    Type recExpression();
    Type recTerm();
    Type recFactor();

    MySema sema;
};

#endif 