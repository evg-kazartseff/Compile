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
    hashTable->getChlidScope();

    std::string end_true = Singleton<MarkGenerator>::getInstance()->Generate();
    int stack = this->asmVars->getStack();

    this->write_adapter->Print("\tpushl %ecx\n");
    asmVars->IncStack(INT_SIZE);
    this->write_adapter->Print("\tmovl %esp, %ecx\n");

    this->Statement->Dfs(); // a > 2 + 3 ==> reslut ==> stack (1/0)
    this->write_adapter->Print("\tpopl %eax\n");
    this->write_adapter->Print("cmpl %eax, $1\n");
    this->write_adapter->Print("\tjnz " + end_true + "\n");
    this->asmVars->DecStack(INT_SIZE);

    this->Body->Dfs();

    this->write_adapter->Print(end_true + ":\n");
    if (this->Else) {
        this->Else->Dfs();
    }

    this->write_adapter->Print("\tmovl %ecx, %esp\n");
    this->write_adapter->Print("\tpopl %ecx\n");
    this->asmVars->setStack(stack);

    hashTable->deleteThisScope();
}

std::string AST::ElseAST::Generate_code()
{
    return std::__cxx11::string();
}

void AST::ElseAST::Dfs()
{
    this->Body->Dfs();
}