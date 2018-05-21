//
// Created by direnol on 20.05.18.
//
#include "../../inc/Ast.h"

std::string AST::CallFuncAST::Generate_code()
{
    std::string str = "\tcall " + this->Id + "\n";
    auto it = this->hashTable->LookupEntry(this->Id);
    if ((it != nullptr) && (it->getType() == asmVars->getIntType())) {
        str += "\tpushl %eax\n";
        asmVars->IncStack(static_cast<int>(asmVars->getIntType()));
    }
    return str;
}

void AST::CallFuncAST::Dfs()
{
    this->Args->Dfs();
    this->write_adapter->Print(Generate_code());
}


std::string AST::ArgListAST::Generate_code()
{
//    return "\tpushl ";
}

void AST::ArgListAST::Dfs()
{
    if (this->Next)
        this->Next->Dfs();

    if (this->Attr) {
        this->Attr->Dfs();
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
    return "\tpushl $" + this->Str + "\n";
}

void AST::StringAST::Dfs() {
    this->write_adapter->Print(this->Generate_code());
}

std::string AST::PrototypeFuncAST::Generate_code() {
    return std::__cxx11::string();
}

void AST::PrototypeFuncAST::Dfs() {

}