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
        asmVars->IncStack(INT_SIZE);
    }
    return str;
}

void AST::CallFuncAST::Dfs()
{
    this->Args->Dfs();
    this->write_adapter->Print(Generate_code());
}

AST::CallFuncAST::~CallFuncAST()
{
    if (this->Args) {
        delete Args;
        Args = nullptr;
    }
}

std::string AST::ArgListAST::Generate_code()
{
//    return "\tpushl ";
}

void AST::ArgListAST::Dfs()
{
    if (this->Next)
        this->Next->Dfs();
    asmVars->DecStack(INT_SIZE);
    if (this->Attr) {
        this->Attr->Dfs();
    }
}

AST::ArgListAST::~ArgListAST()
{
    if (this->Attr) {
        delete Attr;
        Attr = nullptr;
    }
    if (this->Next) {
        delete Next;
        Next = nullptr;
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

AST::ArgsAST::~ArgsAST()
{
    if (this->LList) {
        delete LList;
        LList = nullptr;
    }
}

std::string AST::StringAST::Generate_code()
{
    asmVars->IncStack(INT_SIZE);
    return "\tpushl $" + this->Str + "\n";
}

void AST::StringAST::Dfs()
{
    this->write_adapter->Print(this->Generate_code());
}

std::string AST::PrototypeFuncAST::Generate_code()
{
    return std::__cxx11::string();
}

void AST::PrototypeFuncAST::Dfs()
{
    // TODO maybe add check when call
}

AST::PrototypeFuncAST::PrototypeFuncAST(int ret, std::string id)
        : Ret(ret), Id(std::move(std::move(id)))
{}
