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
    // TODO change hash_table
    std::string start_loop = "LOOP0";
    std::string end_loop = "LOOP1";

    this->write_adapter->Print("\tpushl %ecx\n");
    this->write_adapter->Print("\tmovl %esp, %ecx\n");

    if (Def) this->Def->Dfs();
    this->write_adapter->Print(start_loop + ":\n");

    this->If->Dfs(); // + get mark name
//    this->write_adapter->Print("\tjmp " + end_loop + "\n");

    this->Body->Dfs();
    this->Inc->Dfs();
    this->write_adapter->Print("\tjmp " + start_loop + "\n");

    this->write_adapter->Print(end_loop + ":\n");

    this->write_adapter->Print("\tmovl %ecx, %esp\n");
    this->write_adapter->Print("\tpopl %ecx\n");
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