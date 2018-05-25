//
// Created by direnol on 20.05.18.
//
#include "../../inc/Ast.h"

void AST::Ast::DFS()
{
    if (this->tree == nullptr) return;
    this->tree->Dfs(); /// С этого метода начинать обход дерева
}

void AST::Ast::AddToLink(BaseAST *childe)
{
    this->tree = childe;
}

AST::Ast::~Ast()
{
    if (tree) {
        delete tree;
        tree = nullptr;
    }

}

