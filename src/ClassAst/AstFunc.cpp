//
// Created by direnol on 20.05.18.
//
#include "../../inc/Ast.h"

std::string AST::CallFuncAST::Generate_code()
{
    return "\tcall " + this->Id + "\n";
}

void AST::CallFuncAST::Dfs()
{
    this->Args->Dfs();
    this->write_adapter->Print(Generate_code());
}


std::string AST::ArgListAST::Generate_code()
{
    return "\tpushl ";
}

void AST::ArgListAST::Dfs()
{
    if (this->Next)
        this->Next->Dfs();

    if (this->Attr) {
        this->write_adapter->Print(Generate_code());
        this->Attr->Dfs();
        this->write_adapter->Print("\n");
    }
}

std::string AST::ArgsAST::Generate_code()
{
    return std::__cxx11::string();
}

void AST::ArgsAST::Dfs()
{
    if (this->LList) this->LList->Dfs();
}


std::string AST::StringAST::Generate_code() {
    return std::__cxx11::string();
}

void AST::StringAST::Dfs() {

}
