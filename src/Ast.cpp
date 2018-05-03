//
// Created by evgenii on 31.03.18.
//

#include "../inc/Ast.h"
#include <utility>
#include <typeindex>
#include <search.h>

AST::Ast::Ast() {}

AST::BaseAST* AST::Ast::GetIntNumberExpr(int type, int val) {
    return new IntNumberExprAST(type, val);
}

AST::BaseAST* AST::Ast::GetDoubleNumberExpr(int type, double val) {
    return new DoubleNumberExprAST(type, val);
}

AST::BaseAST* AST::Ast::GetVariableExpr(int type, std::string name) {
    return new VariableExprAST(type, std::move(name));
}

AST::BaseAST* AST::Ast::GetBinaryExpr(int type, char op, BaseAST* lhs, BaseAST* rhs) {
    return new BinaryExprAST(type, op, lhs, rhs);
}

void AST::LinkAST::AddChild(BaseAST* child) {
    this->Childs->push_back(child);
}

std::string AST::LinkAST::Generate_code() {
    return std::__cxx11::string();
}

void AST::LinkAST::Dfs() {

}

void AST::Ast::DFS() {
    if (this->tree == nullptr) return;
    this->tree->Dfs(); /// С этого метода начинать обход дерева
}

AST::BaseAST* AST::Ast::GetEval(int type, std::string id, AST::BaseAST* expr) {
    return new EvalAST(type, id, expr);
}

AST::BaseAST* AST::Ast::GetJump(int type, std::string id) {
    return new JumpAST(type, id);
}

AST::BaseAST* AST::Ast::GetMark(int type, std::string id) {
    return new MarkAST(type, id);
}

void AST::Ast::AddToLink(int type, BaseAST* childe) {
    if (!this->tree)
        this->tree = new LinkAST(type); /// используем синглтон для однократной инициализации
    this->tree->AddChild(childe);
}

AST::BaseAST* AST::Ast::GetVariableDef(int type, std::string name, AST::BaseAST* expr) {
    return new VariableDefAST(type, name, expr);
}

AST::BaseAST* AST::Ast::GetVariableUndef(int type, std::string name) {
    return new VariableUndefAST(type, name);
}

AST::BaseAST* AST::Ast::GetBodyLList(int type, AST::BaseAST* next, AST::BaseAST* attr) {
    return new BodyLListAST(type, next, attr);
}

AST::BaseAST* AST::Ast::GetBody(int type, AST::BaseAST* llist) {
    return new BodyAST(type, llist);
}

std::string AST::IntNumberExprAST::Generate_code() {
    return std::__cxx11::string();
}

void AST::IntNumberExprAST::Dfs() {
    std::cout << this->Val << std::endl;
    //this->Generate_code();
}

std::string AST::DoubleNumberExprAST::Generate_code() {
    return std::__cxx11::string();
}

void AST::DoubleNumberExprAST::Dfs() {
    std::cout << this->Val << std::endl;
    //this->Generate_code();
}

std::string AST::VariableExprAST::Generate_code() {
    return std::__cxx11::string();
}

void AST::VariableExprAST::Dfs() {
    std::cout << this->Name << std::endl;
    ///this->Generate_code();
}

std::string AST::BinaryExprAST::Generate_code() {
    return std::__cxx11::string();
}

void AST::BinaryExprAST::Dfs() {
    this->LHS->Dfs();
    this->RHS->Dfs();
}

std::string AST::CallExprAST::Generate_code() {
    return std::__cxx11::string();
}

void AST::CallExprAST::Dfs() {

}

std::string AST::EvalAST::Generate_code() {
    return std::__cxx11::string();
}

void AST::EvalAST::Dfs() {

}

std::string AST::JumpAST::Generate_code() {
    return std::__cxx11::string();
}

void AST::JumpAST::Dfs() {

}

std::string AST::MarkAST::Generate_code() {
    return JumpAST::Generate_code();
}

void AST::MarkAST::Dfs() {
    JumpAST::Dfs();
}

std::string AST::VariableUndefAST::Generate_code() {
    return std::__cxx11::string();
}

void AST::VariableUndefAST::Dfs() {

}

std::string AST::VariableDefAST::Generate_code() {
    return VariableUndefAST::Generate_code();
}

void AST::VariableDefAST::Dfs() {
    VariableUndefAST::Dfs();
}

std::string AST::BodyAST::Generate_code() {
    return std::__cxx11::string();
}

void AST::BodyAST::Dfs() {

}

std::string AST::BodyLListAST::Generate_code() {
    return std::__cxx11::string();
}

void AST::BodyLListAST::Dfs() {

}
