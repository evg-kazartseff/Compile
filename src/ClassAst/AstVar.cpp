//
// Created by direnol on 20.05.18.
//

#include "../../inc/Ast.h"


std::string AST::IntNumberExprAST::Generate_code()
{
    std::string str = "$" + std::to_string(Val);
    return str;
}

void AST::IntNumberExprAST::Dfs()
{
//    std::cout << ' ' << this->Val << ' ';
    this->write_adapter->Print(this->Generate_code());
}

std::string AST::DoubleNumberExprAST::Generate_code()
{
    std::string str = "$" + std::to_string(Val);
    return str;
}

void AST::DoubleNumberExprAST::Dfs()
{
//    std::cout << ' ' << this->Val << ' ';
    this->write_adapter->Print(this->Generate_code());
}


std::string AST::VariableUndefAST::Generate_code()
{
    int size = this->Type == this->asmVars->getIntType() ? 4 : 8;
    this->asmVars->IncStack(size);
    this->hashTable->setAddr(this->Name, this->asmVars->getStack());
    std::string code = "\tsubl $" + std::to_string(size) + ", %esp\n";
    return code;
}

void AST::VariableUndefAST::Dfs()
{
    std::stringstream str;
//    std::cout << "Undefvar " << this->Name.c_str() << std::endl;
    str << this->Generate_code();
    this->write_adapter->Print(str.str());
}

std::string AST::VariableDefAST::Generate_code()
{
    return VariableUndefAST::Generate_code();
}

void AST::VariableDefAST::Dfs()
{
    std::stringstream str;
    VariableUndefAST::Dfs();
    this->Expr->Dfs();
    this->write_adapter->Print(str.str());
}