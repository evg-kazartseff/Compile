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
    if (!this->tree)
        this->tree = new LinkAST(); /// используем синглтон для однократной инициализации
    this->tree->AddChild(childe);
}

