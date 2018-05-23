//
// Created by direnol on 20.05.18.
//
#include "../../inc/Ast.h"

void AST::LinkAST::AddChild(BaseAST *child)
{
    this->Childs->push_back(child);
}

std::string AST::LinkAST::Generate_code()
{
    return std::__cxx11::string();
}

void AST::LinkAST::Dfs()
{
    for (auto it: *this->Childs) {
        if (it) it->Dfs();
    }
}

AST::LinkAST::~LinkAST()
{
    if (this->Childs) {
        for (auto it: *this->Childs) {
            delete it;
        }
        delete Childs;
        Childs = nullptr;
    }
}
