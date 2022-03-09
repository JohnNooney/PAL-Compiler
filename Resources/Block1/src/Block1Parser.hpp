//
//  Block1Parser.hpp - Full-fledged compiler driver demo for the Block1 Language
//  CompilerKit
//
//  Created by Amy Parent on 09/11/2021.
//
#ifndef _BLOCK1PARSER_HPP_
#define _BLOCK1PARSER_HPP_

#include "CompilerKit/Parser.hpp"
#include "Block1Sema.hpp"
using namespace CompilerKit;

class Block1Parser : public RecoveringParser {
public:
    
    Block1Parser(Scanner& scanner) : RecoveringParser(scanner), sema(*this) {}
    virtual ~Block1Parser() {}
    
    bool compile();
    
private:
    
    void recDeclarations();
    void recBlock();
    void recStatement();
    void recStatementList();
    void recLet();
    void recFor();
    
    // In this parser, we need to drive the semantic analyser, so we return types here.
    Type recExpression();
    Type recTerm();
    Type recFactor();
    
    Block1Sema sema;
};

#endif /* ifndef _BLOCK1PARSER_HPP_ */