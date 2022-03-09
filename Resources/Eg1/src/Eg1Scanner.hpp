//===--------------------------------------------------------------------------------------------===
// Eg1Scanner.hpp - Eg1 example FMS-based scanner.
//
// Created by Amy Parent <amy@amyparent.com>
// Copyright (c) 2022 Amy Parent
// Licensed under the MIT License
//===--------------------------------------------------------------------------------------------===
#ifndef EG1_SCANNER
#define EG1_SCANNER

#include "CompilerKit/Scanner.hpp"

using namespace CompilerKit;

class Eg1Scanner : public Scanner {
public:
    Eg1Scanner(std::istream& in) : Scanner(in) {}
    virtual ~Eg1Scanner() {}
    
    Token lex() override;
};

#endif /* end of include guard: EG1_SCANNER */
