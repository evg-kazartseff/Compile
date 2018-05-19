//
// Created by evgenii on 21.04.18.
//

#include "../inc/Asm.h"

ASM_GEN::ASM_GEN(const std::string &filename, AST::Ast *ast)
{
    this->file_name = filename;
    this->ast = ast;
}

void ASM_GEN::Generate()
{
    this->ast->SetFile(this->file_name);
    this->ast->DFS();
}
