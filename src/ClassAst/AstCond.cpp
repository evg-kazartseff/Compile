//
// Created by direnol on 20.05.18.
//
#include "../../inc/Ast.h"

std::string AST::JumpAST::Generate_code()
{
    return "\tjmp " + this->Id + "\n";
}

void AST::JumpAST::Dfs()
{
//    std::cout << "Goto " << Id << std::endl;
    write_adapter->Print(Generate_code());
}

std::string AST::MarkAST::Generate_code()
{
    return this->Id + ":\n";
}

void AST::MarkAST::Dfs()
{
//    std::cout << "Set " + Id << std::endl;
    write_adapter->Print(Generate_code());
}

