//
// Created by direnol on 20.05.18.
//
#include "../../inc/Ast.h"

AST::BaseAST *AST::Ast::GetIntNumberExpr(int val)
{
    return new IntNumberExprAST(val);
}

AST::BaseAST *AST::Ast::GetDoubleNumberExpr(double val)
{
    return new DoubleNumberExprAST(val);
}

AST::BaseAST *AST::Ast::GetVariableExpr(std::string name)
{
    return new VariableExprAST(std::move(name));
}

AST::BaseAST *AST::Ast::GetBinaryExpr(char op, BaseAST *lhs, BaseAST *rhs)
{
    return new BinaryExprAST(op, lhs, rhs);
}

AST::BaseAST *AST::Ast::GetEval(std::string id, AST::BaseAST *expr)
{
    return new EvalAST(std::move(id), expr);
}

AST::BaseAST *AST::Ast::GetJump(std::string id)
{
    return new JumpAST(std::move(id));
}

AST::BaseAST *AST::Ast::GetMark(std::string id)
{
    return new MarkAST(std::move(id));
}


AST::BaseAST *AST::Ast::GetVariableDef(int type, std::string name, BaseAST *expr)
{
    return new VariableDefAST(type, std::move(name), expr);
}

AST::BaseAST *AST::Ast::GetVariableUndef(int type, std::string name)
{
    return new VariableUndefAST(type, std::move(name));
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

AST::BaseAST *AST::Ast::GetLogicExpr(char op, AST::BaseAST *lhs, AST::BaseAST *rhs)
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

AST::BaseAST *AST::Ast::GetUnary(char operation, AST::BaseAST *id)
{
    return new UnaryAST(operation, id);
}

AST::BaseAST *AST::Ast::GetArgList(AST::BaseAST *next, AST::BaseAST *attr)
{
    return new ArgListAST(next, attr);
}

AST::BaseAST *AST::Ast::GetArgs(AST::BaseAST *llist)
{
    return new ArgsAST(llist);
}

AST::BaseAST *AST::Ast::GetCallFunc(std::string id, AST::BaseAST *args)
{
    return new CallFuncAST(std::move(id), args);
}

AST::BaseAST *AST::Ast::GetReturn(AST::BaseAST *val)
{
    return new ReturnAST(val);
}

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

AST::BaseAST* AST::Ast::GetString(const std::string& str, const std::string &format) {
    return new StringAST(str, format);
}

AST::BaseAST* AST::Ast::GetPrototypeFunc(int ret, const std::string &id) {
    return new PrototypeFuncAST(ret, id);
}