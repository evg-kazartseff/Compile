//
// Created by direnol on 25.05.18.
//

#ifndef COMPILE_ASTEXPR_H
#define COMPILE_ASTEXPR_H

#include "AstBase.h"

namespace AST {
    /// BinaryExprAST - Класс узла выражения для бинарных операторов.
    class BinaryExprAST : public BaseAST {
    public:
        char Op;
        BaseAST *LHS, *RHS;
    public:
        BinaryExprAST(char op, BaseAST *lhs, BaseAST *rhs)
                : Op(op), LHS(lhs), RHS(rhs)
        {}

        ~BinaryExprAST() override;

        std::string Generate_code() final;

        void Dfs() final;
    };

    class LogicExprAST : public BaseAST {
    public:
        char Op;
        BaseAST *LHS, *RHS;
    public:
        LogicExprAST(char op, BaseAST *lhs, BaseAST *rhs)
                : Op(op), LHS(lhs), RHS(rhs)
        {}

        ~LogicExprAST() override;

        std::string Generate_code() final;

        void Dfs() final;
    };

    /// EvalAST - Класс переопределения переменной
    class EvalAST : public BaseAST {
    public:
        std::string Id;
        BaseAST *Expr;
    public:
        EvalAST(std::string id, BaseAST *expr)
                : Id(std::move(id)), Expr(expr)
        {}

        ~EvalAST() override;

        std::string Generate_code() final;

        void Dfs() final;

    };

    class UnaryAST : public BaseAST {
        char Operation;
        BaseAST *Operand;
    public:
        UnaryAST(char operation, BaseAST *id)
                : Operation(operation), Operand(id)
        {}

        ~UnaryAST() override;

        std::string Generate_code() final;

        void Dfs() final;
    };
};
#endif //COMPILE_ASTEXPR_H
