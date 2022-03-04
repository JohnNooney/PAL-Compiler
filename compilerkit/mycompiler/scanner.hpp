#include "../src/CompilerKit/Scanner.hpp"
using namespace CompilerKit;

class MyScanner : public Scanner {
public:
    MyScanner(std::istream& source) : Scanner(source){}
    virtual ~MyScanner() {}

        Token lex() override;
    }
};
