//
//  Block1PureParser.hpp - Full-fledged compiler driver demo for the Block1 Language
//  CompilerKit
//
//  Created by Amy Parent on 09/11/2021.
//
#ifndef _BLOCK1PUREPARSER_HPP_
#define _BLOCK1PUREPARSER_HPP_

#include "CompilerKit/Parser.hpp"
using namespace CompilerKit;

class Block1PureParser : public Parser {
public:
    
    Block1PureParser(Scanner& scanner) : Parser(scanner) {}
    virtual ~Block1PureParser() {}
    
    void compile();
    
private:
    
    void recDeclarations();
    void recBlock();
    void recStatement();
    void recStatementList();
    void recLet();
    void recFor();
    void recExpression();
    void recTerm();
    void recFactor();
};

#endif /* ifndef _BLOCK1PUREPARSER_HPP_ */