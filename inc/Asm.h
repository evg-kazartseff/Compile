//
// Created by evgenii on 21.04.18.
//

#ifndef COMPILE_ASM_H
#define COMPILE_ASM_H

#include <fstream>
#include "Ast.h"
#include "WriteAdapter.h"
#include "StringNameGenerator.h"

class ASM_GEN {
private:
    WriteAdapter* writeAdapter;
    std::string file_name;
    AST::Ast *ast;
public:
    ASM_GEN(const std::string &filename, AST::Ast *ast);
    void Generate();
};

#endif //COMPILE_ASM_H
