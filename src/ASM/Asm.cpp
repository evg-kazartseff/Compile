//
// Created by evgenii on 21.04.18.
//

#include <sstream>
#include "../../inc/asm/Asm.h"

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
    str << "\t.data\n";
    for (auto it : Singleton<FormatAcum>::getInstance()->GetAcum()) {
        str << it.first + ":\n\t.string " << it.second << "\n\t.set "
            << it.first + "_size, . - " << it.first << " - 1\n";
    }

    str << "\n\t.text\n"
           "\t.global main\n"
           "\t.type main, @function\n"
           "main:\n"
           "\tpushl %ebp\n"
           "\tmovl %esp, %ebp\n\n";

    this->writeAdapter->Print(str.str());
    this->ast->DFS();

    str.str("");
    str << "\n\tmovl $0, %eax\n"
           "\tmovl %ebp, %esp\n"
           "\tpopl %ebp\n"
           "\tret\n";
    this->writeAdapter->Print(str.str());
}
