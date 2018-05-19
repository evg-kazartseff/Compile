//
// Created by evgenii on 21.04.18.
//

#ifndef COMPILE_ASM_H
#define COMPILE_ASM_H

#include <fstream>
#include "Ast.h"

class ASM_GEN {
    friend AST::Ast;
private:
    std::string file_name;
    AST::Ast *ast;
public:
    ASM_GEN(const std::string &filename, AST::Ast *ast);

    void Generate();
};

#endif //COMPILE_ASM_H
