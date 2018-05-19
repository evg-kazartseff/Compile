//
// Created by evgenii on 21.04.18.
//

#include <sstream>
#include "../inc/Asm.h"

ASM_GEN::ASM_GEN(const std::string &filename, AST::Ast *ast)
{
    this->writeAdapter = Singleton<WriteAdapter>::getInstance();
    this->file_name = filename;
    this->writeAdapter->SetFile(this->file_name);
    this->ast = ast;
}

void ASM_GEN::Generate()
{
    std::stringstream str;
    str << "\t.file pipiska" << std::endl;
    // set variable;
    str << "\t.text\n"
                 "\t.global main\n"
                 "\ttype main, @function\n"
                 "main:\n "
                 ".LFB0:\n" << std::endl;
    this->writeAdapter->Print(str.str());
    this->ast->DFS();
    str.str("");
    str << "\n.LFE0:\n"
                 "\t.size main, .-main\n"
                 "\t.ident \"Tutanh\"\n"
                 "\t.section .note.GNU-stack,\"\",@progbits" << std::endl;
    this->writeAdapter->Print(str.str());
}
