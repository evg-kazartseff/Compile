//
// Created by evgenii on 31.03.18.
//

#include "../../inc/Ast.h"

AST::BaseAST::BaseAST()
{
    this->write_adapter = Singleton<WriteAdapter>::getInstance();
    this->asmVars = Singleton<AsmVars>::getInstance();
    this->hashTable = Singleton<HashTable>::getInstance();
}

std::string AST::BodyAST::Generate_code()
{
    return std::__cxx11::string();
}

void AST::BodyAST::Dfs()
{
    if (this->LList) this->LList->Dfs();
}

std::string AST::BodyLListAST::Generate_code()
{
    return std::__cxx11::string();
}

void AST::BodyLListAST::Dfs()
{
    this->Attr->Dfs();
    if (this->Next)
        this->Next->Dfs();
}

std::string AST::IfAST::Generate_code()
{
    return std::__cxx11::string();
}

void AST::IfAST::Dfs()
{
    std::cout << "if (";
    this->Statement->Dfs();
    std::cout << ") {\n";
    this->Body->Dfs();
    std::cout << "}";
    if (this->Else) this->Else->Dfs();
}

std::string AST::ElseAST::Generate_code()
{
    return std::__cxx11::string();
}

void AST::ElseAST::Dfs()
{
    std::cout << " else {\n";
    this->Body->Dfs();
    std::cout << "}\n";
}

std::string AST::LoopAST::Generate_code()
{
    return std::__cxx11::string();
}

void AST::LoopAST::Dfs()
{
    std::cout << "for (";
    this->Def->Dfs();
    std::cout << "; ";
    this->If->Dfs();
    std::cout << "; ";
    this->Inc->Dfs();
    std::cout << ") {\n";
    this->Body->Dfs();
    std::cout << "}\n";
}

std::string AST::ReturnAST::Generate_code()
{
    std::string str = "\n\tmovl " +
                      this->Val->Generate_code() +
                      ", %eax\n"
                      "\tmovl %ebp, %esp\n"
                      "\tpopl %ebp\n"
                      "\tret\n";
    return str;
}

void AST::ReturnAST::Dfs()
{
    this->write_adapter->Print(Generate_code());
}