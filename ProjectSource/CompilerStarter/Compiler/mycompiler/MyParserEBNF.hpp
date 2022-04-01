#include "../CompilerKit/Parser.hpp"
#include "MySema.hpp"
using namespace CompilerKit;


class MyParserEBNF : public Parser {
public:

    MyParserEBNF(Scanner& scanner) : Parser(scanner), sema(*this) {}
    virtual ~MyParserEBNF() {}

    bool compile();
    //void recStarter();

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