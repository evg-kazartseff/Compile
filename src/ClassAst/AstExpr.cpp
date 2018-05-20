//
// Created by direnol on 20.05.18.
//

#include "../../inc/Ast.h"

std::string AST::VariableExprAST::Generate_code()
{
    return std::__cxx11::string();
}


void AST::VariableExprAST::Dfs()
{
    printf(" %s ", this->Name.c_str());
    ///this->Generate_code();
}

std::string AST::BinaryExprAST::Generate_code()
{
    // set result
    return std::__cxx11::string();
}

void AST::BinaryExprAST::Dfs()
{
    this->LHS->Dfs();
    // result left

    this->RHS->Dfs();
    // result right
}


std::string AST::EvalAST::Generate_code()
{
    return std::__cxx11::string();
}

void AST::EvalAST::Dfs()
{
    printf("%s =", this->Id.c_str());
    this->Expr->Dfs();
    puts("");
}

std::string AST::UnaryAST::Generate_code()
{
    return std::__cxx11::string();
}

void AST::UnaryAST::Dfs()
{
    std::cout << "(" << this->Operation;
    this->Operand->Dfs();
    std::cout << ")";
}