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
    write_adapter->Print("\t#Jump start\n");

    write_adapter->Print(Generate_code());
    write_adapter->Print("\t#Jump end\n");
}

std::string AST::MarkAST::Generate_code()
{
    return this->Id + ":\n";
}

void AST::MarkAST::Dfs()
{
//    std::cout << "Set " + Id << std::endl;
    write_adapter->Print("\t#Mark set start\n");
    write_adapter->Print(Generate_code());
    write_adapter->Print("\t#Mark set end\n");
}

std::string AST::IfAST::Generate_code()
{
    return std::__cxx11::string();
}

void AST::IfAST::Dfs()
{
    write_adapter->Print("\t#If start\n");
    hashTable->getChlidScope();

    std::string end_true = Singleton<MarkGenerator>::getInstance()->Generate();
    std::string end_if = Singleton<MarkGenerator>::getInstance()->Generate();
    int stack = this->asmVars->getStack();

    this->Statement->Dfs(); // a > 2 + 3 ==> reslut ==> stack (1/0)
    this->write_adapter->Print("\tpopl %eax\n");
    this->write_adapter->Print("\tmovl $0, %ebx\n");
    this->write_adapter->Print("\tcmpl %eax, %ebx\n");
    this->write_adapter->Print("\tje " + end_true + "\n");
    this->asmVars->DecStack(INT_SIZE);

    this->Body->Dfs();
    this->write_adapter->Print("\tjmp " + end_if + "\n");
    this->write_adapter->Print(end_true + ":\n");
    if (this->Else) {
        hashTable->popScope();
        this->Else->Dfs();
    }

    this->write_adapter->Print(end_if + ":\n");
    int new_stack = asmVars->getStack();
    this->write_adapter->Print("\taddl $" + std::to_string(new_stack - stack) + ", %esp\n");
    this->asmVars->DecStack(new_stack - stack);
    if (!this->Else)
        hashTable->popScope();
    write_adapter->Print("\t#If end\n");
}

AST::IfAST::~IfAST()
{
    if (this->Body) {
        delete Body;
        Body = nullptr;
    }
    if (this->Statement) {
        delete Statement;
        Statement = nullptr;
    }
    if (this->Else) {
        delete Else;
        Else = nullptr;
    }
}

std::string AST::ElseAST::Generate_code()
{
    return std::__cxx11::string();
}

void AST::ElseAST::Dfs()
{
    write_adapter->Print("\t#Else start\n");
    hashTable->getChlidScope();
    this->Body->Dfs();
    write_adapter->Print("\t#Else end\n");
}

AST::ElseAST::~ElseAST()
{
    if (Body) {
        delete Body;
        Body = nullptr;
    }
}
