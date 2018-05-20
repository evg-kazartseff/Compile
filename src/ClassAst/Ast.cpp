//
// Created by evgenii on 31.03.18.
//
#include "../../inc/Ast.h"


std::string AST::CallFuncAST::Generate_code()
{
    return std::__cxx11::string();
}

void AST::CallFuncAST::Dfs()
{

}

std::string AST::JumpAST::Generate_code()
{
    return std::__cxx11::string();
}

void AST::JumpAST::Dfs()
{
    printf("Goto %s\n", this->Id.c_str());
}

std::string AST::MarkAST::Generate_code()
{
    return JumpAST::Generate_code();
}

void AST::MarkAST::Dfs()
{
    printf("New Mark %s\n", this->Id.c_str());
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

std::string AST::LogicExprAST::Generate_code()
{
    return std::__cxx11::string();
}

void AST::LogicExprAST::Dfs()
{
    std::cout << this->Op << " ";
    this->LHS->Dfs();
    this->RHS->Dfs();
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


std::string AST::ArgListAST::Generate_code()
{
    return std::__cxx11::string();
}

void AST::ArgListAST::Dfs()
{

}

std::string AST::ArgsAST::Generate_code()
{
    return std::__cxx11::string();
}

void AST::ArgsAST::Dfs()
{

}

std::string AST::ReturnAST::Generate_code()
{
    return std::__cxx11::string();
}

void AST::ReturnAST::Dfs()
{

}
