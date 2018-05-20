//
// Created by direnol on 20.05.18.
//
#include "../../inc/Ast.h"

std::string AST::CallFuncAST::Generate_code()
{
    return "\n\tcall " + this->Id + "\n";
}

void AST::CallFuncAST::Dfs()
{
    this->Args->Dfs();
    this->write_adapter->Print(Generate_code());
}


std::string AST::ArgListAST::Generate_code()
{
    return std::__cxx11::string();
}

void AST::ArgListAST::Dfs()
{
    if (this->Next) this->Next->Dfs();
    if (this->Attr) this->Attr->Dfs();
}

std::string AST::ArgsAST::Generate_code()
{
    return std::__cxx11::string();
}

void AST::ArgsAST::Dfs()
{
    if (this->LList) this->LList->Dfs();
}