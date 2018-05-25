//
// Created by direnol on 25.05.18.
//

#ifndef COMPILE_ASTVAR_H
#define COMPILE_ASTVAR_H

#include "AstBase.h"

namespace AST {

    /// IntNumberExprAST - Класс узла выражения для числовых литералов (Например, "1").
    class IntNumberExprAST : public BaseAST {
    public:
        int Val;
    public:
        explicit IntNumberExprAST(int val)
                : Val(val), BaseAST()
        {}

        ~IntNumberExprAST() override = default;

        std::string Generate_code() final;

        void Dfs() final;
    };

    /// DoubleNumberExprAST - Класс узла выражения для числовых литералов (Например, "1.2").
    class DoubleNumberExprAST : public BaseAST {
    public:
        double Val;
    public:
        explicit DoubleNumberExprAST(double val)
                : Val(val)
        {}

        ~DoubleNumberExprAST() override = default;

        std::string Generate_code() final;

        void Dfs() final;
    };

    /// VariableExprAST - Класс узла выражения для переменных (например, "a").
    class VariableExprAST : public BaseAST {
        std::string Name;
    public:
        explicit VariableExprAST(std::string name)
                : Name(std::move(name))
        {}

        ~VariableExprAST() override = default;

        std::string Generate_code() final;

        void Dfs() final;

    };

    class VariableUndefAST : public BaseAST {
    protected:
        int Type;
        std::string Name;
    public:
        VariableUndefAST(int type, std::string name)
                : Type(type), Name(std::move(name)), BaseAST()
        {}

        ~VariableUndefAST() override = default;

        std::string Generate_code() override;

        void Dfs() override;
    };

    class VariableDefAST : public VariableUndefAST {
        BaseAST *Expr;
    public:
        VariableDefAST(int type, std::string name, BaseAST *expr)
                : VariableUndefAST(type, std::move(name)), Expr(expr)
        {}

        ~VariableDefAST() override;

        std::string Generate_code() final;

        void Dfs() final;
    };

    class StringAST : public BaseAST {
        std::string Str;
        std::string Format;
    public:
        explicit StringAST(std::string str, std::string format)
                : Str(std::move(str)), Format(std::move(format))
        {}

        ~StringAST() override = default;

        std::string Generate_code() final;

        void Dfs() final;
    };
};

#endif //COMPILE_ASTVAR_H
