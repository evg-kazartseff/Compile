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

std::string AST::IfAST::Generate_code()
{
    return std::__cxx11::string();
}

void AST::IfAST::Dfs()
{
    std::string start_else = Singleton<MarkGenerator>::getInstance()->Generate();
    std::string end_if = Singleton<MarkGenerator>::getInstance()->Generate();
    int stack = this->asmVars->getStack();

    this->write_adapter->Print("\tpushl %ecx\n");
    this->write_adapter->Print("\tmovl %esp, %ecx\n");

    this->Statement->Dfs();
    this->Body->Dfs();

    if (this->Else) {
        this->write_adapter->Print(start_else + ":\n");
        this->Else->Dfs();
    }

    this->write_adapter->Print(end_if + ":\n");
    this->write_adapter->Print("\tmovl %ecx, %esp\n");
    this->write_adapter->Print("\tpopl %ecx\n");
    this->asmVars->setStack(stack);
}

std::string AST::ElseAST::Generate_code()
{
    return std::__cxx11::string();
}

void AST::ElseAST::Dfs()
{
    this->Body->Dfs();
}