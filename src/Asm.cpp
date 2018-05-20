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
    str << "\t.data\n"
           "fmtd:\t.string \"%d\"\n"
           "fmtld:\t.string \"%ld\"\n"
           "fmtf:\t.string \"%f\"\n"
           "fmtlf:\t.string \"%lf\"\n"
           "fmts:\t.string \" \"\n"
           "fmtn:\t.string \"\\n\"\n"
           "\n\t.text\n"
           "\t.global main\n"
           "\t.type main, @function\n"
           "main:\n"
           "\tpushl %ebp\n"
           "\tmovl %esp, %ebp\n\n";

    this->writeAdapter->Print(str.str());
    this->ast->DFS();

    str.str("");
    str << "\tmovl %ebp, %esp\n"
           "\tpopl %ebp\n"
           "\tret\n";
    this->writeAdapter->Print(str.str());
}
