#include "../CompilerKit/Parser.hpp"
using namespace CompilerKit;
class MyParser : public Parser {
public:

    MyParser(Scanner& scanner) : Parser(scanner) {}
    virtual ~MyParser() {}

    void recStarter();

private:
    void recDeclarations();
    void recBlock();
};