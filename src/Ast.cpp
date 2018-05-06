//
// Created by evgenii on 31.03.18.
//

#include "../inc/Ast.h"
#include <typeindex>

AST::Ast::Ast()
{}

AST::BaseAST *AST::Ast::GetIntNumberExpr(int type, int val)
{
    return new IntNumberExprAST(type, val);
}

AST::BaseAST *AST::Ast::GetDoubleNumberExpr(int type, double val)
{
    return new DoubleNumberExprAST(type, val);
}

AST::BaseAST *AST::Ast::GetVariableExpr(int type, std::string name)
{
    return new VariableExprAST(type, std::move(name));
}

AST::BaseAST *AST::Ast::GetBinaryExpr(int type, char op, BaseAST *lhs, BaseAST *rhs)
{
    return new BinaryExprAST(type, op, lhs, rhs);
}

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

void AST::Ast::DFS()
{
    if (this->tree == nullptr) return;
    this->tree->Dfs(); /// С этого метода начинать обход дерева
}

AST::BaseAST *AST::Ast::GetEval(int type, std::string id, AST::BaseAST *expr)
{
    return new EvalAST(type, id, expr);
}

AST::BaseAST *AST::Ast::GetJump(int type, std::string id)
{
    return new JumpAST(type, id);
}

AST::BaseAST *AST::Ast::GetMark(int type, std::string id)
{
    return new MarkAST(type, id);
}

void AST::Ast::AddToLink(int type, BaseAST *childe)
{
    if (!this->tree)
        this->tree = new LinkAST(type); /// используем синглтон для однократной инициализации
    this->tree->AddChild(childe);
}

AST::BaseAST *AST::Ast::GetVariableDef(int type, std::string name, AST::BaseAST *expr)
{
    return new VariableDefAST(type, name, expr);
}

AST::BaseAST *AST::Ast::GetVariableUndef(int type, std::string name)
{
    return new VariableUndefAST(type, name);
}

AST::BaseAST *AST::Ast::GetBodyLList(int type, AST::BaseAST *next, AST::BaseAST *attr)
{
    return new BodyLListAST(type, next, attr);
}

AST::BaseAST *AST::Ast::GetBody(int type, AST::BaseAST *llist)
{
    return new BodyAST(type, llist);
}

std::string AST::IntNumberExprAST::Generate_code()
{
    return std::__cxx11::string();
}

void AST::IntNumberExprAST::Dfs()
{
    printf(" %d ", this->Val);
    //this->Generate_code();
}

std::string AST::DoubleNumberExprAST::Generate_code()
{
    return std::__cxx11::string();
}

void AST::DoubleNumberExprAST::Dfs()
{
    printf(" %lf ", this->Val);
    //this->Generate_code();
}

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
    return std::__cxx11::string();
}

void AST::BinaryExprAST::Dfs()
{
    printf("(%c ", this->Op);
    this->LHS->Dfs();
    this->RHS->Dfs();
    printf(")");
}

std::string AST::CallExprAST::Generate_code()
{
    return std::__cxx11::string();
}

void AST::CallExprAST::Dfs()
{

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

std::string AST::JumpAST::Generate_code()
{
    return std::__cxx11::string();
}

void AST::JumpAST::Dfs()
{
    printf("Goto %s\n", this->Id.c_str());
}

std::string AST::MarkAST::Generate_code()
{
    return JumpAST::Generate_code();
}

void AST::MarkAST::Dfs()
{
    printf("New Mark %s\n", this->Id.c_str());
}

std::string AST::VariableUndefAST::Generate_code()
{
    return std::__cxx11::string();
}

void AST::VariableUndefAST::Dfs()
{
    printf("UndefVar: (%d) %s\n", this->Type, this->Name.c_str());
}

std::string AST::VariableDefAST::Generate_code()
{
    return VariableUndefAST::Generate_code();
}

void AST::VariableDefAST::Dfs()
{
    printf("(%d) %s = ", this->Type, this->Name.c_str());
    this->Expr->Dfs();
    puts("");
}

std::string AST::BodyAST::Generate_code()
{
    return std::__cxx11::string();
}

void AST::BodyAST::Dfs()
{

}

std::string AST::BodyLListAST::Generate_code()
{
    return std::__cxx11::string();
}

void AST::BodyLListAST::Dfs()
{

}
