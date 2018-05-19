//
// Created by evgenii on 31.03.18.
//

#include "../inc/Ast.h"
#include <typeindex>

AST::Ast::Ast()
{}

AST::BaseAST *AST::Ast::GetIntNumberExpr(int val)
{
    return new IntNumberExprAST(val);
}

AST::BaseAST *AST::Ast::GetDoubleNumberExpr(double val)
{
    return new DoubleNumberExprAST(val);
}

AST::BaseAST*AST::Ast::GetVariableExpr(std::string name)
{
    return new VariableExprAST(std::move(name));
}

AST::BaseAST *AST::Ast::GetBinaryExpr( char op, BaseAST *lhs, BaseAST *rhs)
{
    return new BinaryExprAST(op, lhs, rhs);
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

AST::BaseAST *AST::Ast::GetEval(std::string id, AST::BaseAST *expr)
{
    return new EvalAST(id, expr);
}

AST::BaseAST *AST::Ast::GetJump(std::string id)
{
    return new JumpAST(id);
}

AST::BaseAST *AST::Ast::GetMark(std::string id)
{
    return new MarkAST(id);
}

void AST::Ast::AddToLink(BaseAST *childe)
{
    if (!this->tree)
        this->tree = new LinkAST(); /// используем синглтон для однократной инициализации
    this->tree->AddChild(childe);
}

AST::BaseAST* AST::Ast::GetVariableDef(int type, std::string name, BaseAST* expr)
{
    return new VariableDefAST(type, name, expr);
}

AST::BaseAST*AST::Ast::GetVariableUndef(int type, std::string name)
{
    return new VariableUndefAST(type, name);
}

AST::BaseAST *AST::Ast::GetBodyLList(AST::BaseAST *next, AST::BaseAST *attr)
{
    return new BodyLListAST(next, attr);
}

AST::BaseAST *AST::Ast::GetBody(AST::BaseAST *llist)
{
    return new BodyAST(llist);
}

AST::BaseAST *AST::Ast::GetIf(AST::BaseAST *statement, AST::BaseAST *body, AST::BaseAST *els)
{
    return new IfAST(statement, body, els);
}

AST::BaseAST *AST::Ast::GetLogicExpr( char op, AST::BaseAST *lhs, AST::BaseAST *rhs)
{
    return new LogicExprAST(op, lhs, rhs);
}

AST::BaseAST *AST::Ast::GetElse(AST::BaseAST *body)
{
    return new ElseAST(body);
}

AST::BaseAST *AST::Ast::GetLoop(AST::BaseAST *def, AST::BaseAST *If, AST::BaseAST *inc, AST::BaseAST *body)
{
    return new LoopAST(def, If, inc, body);
}

AST::BaseAST* AST::Ast::GetUnary(char operation, AST::BaseAST* id) {
    return new UnaryAST(operation, id);
}

AST::BaseAST* AST::Ast::GetArgList(AST::BaseAST* next, AST::BaseAST* attr) {
    return new ArgListAST(next, attr);
}

AST::BaseAST* AST::Ast::GetArgs(AST::BaseAST* llist) {
    return new ArgsAST(llist);
}

AST::BaseAST* AST::Ast::GetCallFunc(std::string id, AST::BaseAST* args) {
    return new CallFuncAST(id, args);
}

void AST::Ast::SetFile(const std::string &filename) {
    this->tree->SetFile(filename);
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

std::string AST::CallFuncAST::Generate_code()
{
    return std::__cxx11::string();
}

void AST::CallFuncAST::Dfs()
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
    printf("UndefVar: %s\n", this->Name.c_str());
}

std::string AST::VariableDefAST::Generate_code()
{
    return VariableUndefAST::Generate_code();
}

void AST::VariableDefAST::Dfs()
{
    printf("%s = ", this->Name.c_str());
    this->Expr->Dfs();
    puts("");
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

std::string AST::IfAST::Generate_code()
{
    return std::__cxx11::string();
}

void AST::IfAST::Dfs()
{
    std::cout << "if (";
    this->Statement->Dfs();
    std::cout << ") {\n";
    this->Body->Dfs();
    std::cout << "}";
    if (this->Else) this->Else->Dfs();
}

std::string AST::LogicExprAST::Generate_code()
{
    return std::__cxx11::string();
}

void AST::LogicExprAST::Dfs()
{
    std::cout << this->Op << " ";
    this->LHS->Dfs();
    this->RHS->Dfs();
}

std::string AST::ElseAST::Generate_code()
{
    return std::__cxx11::string();
}

void AST::ElseAST::Dfs()
{
    std::cout << " else {\n";
    this->Body->Dfs();
    std::cout << "}\n";
}

std::string AST::LoopAST::Generate_code()
{
    return std::__cxx11::string();
}

void AST::LoopAST::Dfs()
{
    std::cout << "for (";
    this->Def->Dfs();
    std::cout << "; ";
    this->If->Dfs();
    std::cout << "; ";
    this->Inc->Dfs();
    std::cout << ") {\n";
    this->Body->Dfs();
    std::cout << "}\n";
}

std::string AST::UnaryAST::Generate_code() {
    return std::__cxx11::string();
}

void AST::UnaryAST::Dfs() {
    std::cout << "(" << this->Operation;
    this->Operand->Dfs();
    std::cout << ")";
}

std::string AST::ArgListAST::Generate_code() {
    return std::__cxx11::string();
}

void AST::ArgListAST::Dfs() {

}

std::string AST::ArgsAST::Generate_code() {
    return std::__cxx11::string();
}

void AST::ArgsAST::Dfs() {

}

void AST::BaseAST::SetFile(const std::string& filename) {
    this->file.open(filename.c_str(), std::fstream::out);
}
