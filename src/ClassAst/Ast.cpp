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

std::string AST::LoopAST::Generate_code()
{
    return std::__cxx11::string();
}

void AST::LoopAST::Dfs()
{
    hashTable->getChlidScope();

    std::string start_loop = Singleton<MarkGenerator>::getInstance()->Generate();
    std::string end_loop = Singleton<MarkGenerator>::getInstance()->Generate();
    int stack = this->asmVars->getStack();

    this->write_adapter->Print("\tpushl %ecx\n");
    asmVars->IncStack(INT_SIZE);
    this->write_adapter->Print("\tmovl %esp, %ecx\n");

    if (Def) this->Def->Dfs();
    this->write_adapter->Print(start_loop + ":\n");

    this->If->Dfs();
    this->write_adapter->Print("\tpopl %eax\n");
    this->write_adapter->Print("\tmovl $1, %ebx\n");
    this->write_adapter->Print("\tcmpl %eax, %ebx\n");
    this->write_adapter->Print("\tjz " + end_loop + "\n");
    this->asmVars->DecStack(INT_SIZE);

    this->Body->Dfs();
    this->Inc->Dfs();

    this->write_adapter->Print("\tjmp " + start_loop + "\n");
    this->write_adapter->Print(end_loop + ":\n");

    int new_stack = asmVars->getStack();
    this->write_adapter->Print("\taddl $" + std::to_string(new_stack - stack) + ", %esp\n");
    this->asmVars->DecStack(new_stack - stack);

    hashTable->deleteThisScope();
}

std::string AST::ReturnAST::Generate_code()
{
    std::string str = "\n\tpopl %eax\n"
                      "\tmovl %ebp, %esp\n"
                      "\tpopl %ebp\n"
                      "\tret\n";
    return str;
}

void AST::ReturnAST::Dfs()
{
    this->Val->Dfs();
    this->write_adapter->Print(Generate_code());
}