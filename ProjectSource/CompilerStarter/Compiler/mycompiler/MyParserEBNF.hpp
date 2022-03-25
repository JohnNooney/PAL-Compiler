#include "../CompilerKit/Parser.hpp"
using namespace CompilerKit;

class MyParserEBNF : public Parser {
public:

    MyParserEBNF(Scanner& scanner) : Parser(scanner) {}
    virtual ~MyParserEBNF() {}

    void recStarter();

private:
    void recDeclaration();
    void recStatement();
    void recBlock();
    void recLet();
    void recFor();
    void recExpression();
    void recTerm();
    void recFactor();
};