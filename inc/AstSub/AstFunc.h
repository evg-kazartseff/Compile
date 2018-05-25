//
// Created by direnol on 25.05.18.
//
#ifndef COMPILE_ASTFUNC_H
#define COMPILE_ASTFUNC_H

#include "AstBase.h"

namespace AST {
/// CallExprAST - Класс узла выражения для вызова функции.
    class CallFuncAST : public BaseAST {
        std::string Id;
        BaseAST *Args;
    public:
        CallFuncAST(std::string id, BaseAST *args)
                : Id(std::move(id)), Args(args)
        {}

        ~CallFuncAST() override;

        std::string Generate_code() final;

        void Dfs() final;
    };

    class PrototypeFuncAST : public BaseAST {
        int Ret;
        std::string Id;
    public:
        PrototypeFuncAST(int ret, std::string id);

        ~PrototypeFuncAST() override = default;

        std::string Generate_code() final;

        void Dfs() final;

    };

    class ArgListAST : public BaseAST {
        BaseAST *Next;
        BaseAST *Attr;
    public:
        ArgListAST(BaseAST *next, BaseAST *attr)
                : Next(next), Attr(attr)
        {}

        ~ArgListAST() override;

        std::string Generate_code() final;

        void Dfs() final;
    };

    class ArgsAST : public BaseAST {
        BaseAST *LList = nullptr;
    public:
        explicit ArgsAST(BaseAST *llist)
                : LList(llist)
        {}

        ~ArgsAST() override;

        std::string Generate_code() final;

        void Dfs() final;
    };
};

#endif