//
// Created by evgenii on 31.03.18.
//

#ifndef COMPILE_AST_H
#define COMPILE_AST_H

#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <sstream>
#include "WriteAdapter.h"
#include "Singleton.h"
#include "ASM_VARS.h"
#include "HashTable.h"
#include "StringGenerator.h"
#include <cstring>

namespace AST {
    class Ast;

    /// BaseAST - Базовый класс для всех узлов выражений.
    class BaseAST {
    protected:
        WriteAdapter *write_adapter;
        AsmVars *asmVars;
        HashTable *hashTable;
    public:
        BaseAST();

        virtual ~BaseAST() = default;

        virtual std::string Generate_code() = 0;

        virtual void Dfs() = 0;
    };

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
        bool Is_addr;
    public:
        explicit VariableExprAST(std::string name, bool is_addr = false)
                : Name(std::move(name)), Is_addr(is_addr)
        {}

        ~VariableExprAST() override = default;

        std::string Generate_code() final;

        void Dfs() final;

        void setAddr()
        { this->Is_addr = true; }
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
        bool need_ret;
    public:
        EvalAST(std::string id, BaseAST *expr, bool need = false)
                : Id(std::move(id)), Expr(expr), need_ret(need)
        {}

        ~EvalAST() override;

        std::string Generate_code() final;

        void Dfs() final;

        void SetNeed();
    };

    class JumpAST : public BaseAST {
    public:
        std::string Id;
    public:
        JumpAST() = default;

        explicit JumpAST(std::string id)
                : Id(std::move(id))
        {}

        ~JumpAST() override = default;

        std::string Generate_code() override;

        void Dfs() override;
    };

    class MarkAST : public JumpAST {
    public:
        explicit MarkAST(std::string id)
                : JumpAST(std::move(id))
        {}

        ~MarkAST() override = default;

        std::string Generate_code() final;

        void Dfs() final;
    };

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

    ///LinkAST - Связующий элемент в дереве
    class LinkAST : public BaseAST {
        std::vector<BaseAST *> *Childs = nullptr;
    public:
        LinkAST()
        { this->Childs = new std::vector<BaseAST *>(); }

        explicit LinkAST(std::vector<BaseAST *> *childs)
                : Childs(childs)
        {}

        ~LinkAST() override;

        void AddChild(BaseAST *child);

        std::string Generate_code() final;

        void Dfs() final;
    };

    class BodyLListAST : public BaseAST {
        BaseAST *Next;
        BaseAST *Attr;
    public:
        BodyLListAST(BaseAST *next, BaseAST *attr)
                : Next(next), Attr(attr)
        {}

        ~BodyLListAST() override;

        std::string Generate_code() final;

        void Dfs() final;
    };

    class BodyAST : public BaseAST {
        BaseAST *LList = nullptr;
    public:
        explicit BodyAST(BaseAST *llist)
                : LList(llist)
        {}

        ~BodyAST() override;

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

    /// If else
    class IfAST : public BaseAST {
        BaseAST *Statement;
        BaseAST *Body;
        BaseAST *Else;
    public:
        IfAST(BaseAST *statement, BaseAST *body, BaseAST *els)
                : Statement(statement), Body(body), Else(els)
        {}

        ~IfAST() override;

        std::string Generate_code() final;

        void Dfs() final;
    };

    class ElseAST : public BaseAST {
        BaseAST *Body;
    public:
        explicit ElseAST(BaseAST *body)
                : Body(body)
        {}

        ~ElseAST() override;

        std::string Generate_code() final;

        void Dfs() final;
    };

    class LoopAST : public BaseAST {
        BaseAST *Def;
        BaseAST *If;
        BaseAST *Inc;
        BaseAST *Body;
    public:
        LoopAST(BaseAST *def, BaseAST *If, BaseAST *inc, BaseAST *body)
                : Def(def), If(If), Inc(inc), Body(body)
        {}

        ~LoopAST() override;

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

    class ReturnAST : public BaseAST {
        BaseAST *Val;
    public:
        explicit ReturnAST(BaseAST *val)
                : Val(val)
        {}

        ~ReturnAST() override;

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

    class Ast {
        LinkAST *tree;
    public:
        Ast() = default;

        ~Ast();

        template<class Node, class ...Args>
        BaseAST *Getter(Args... args)
        {
            return new Node(std::forward<Args>(args)...);
        }

        void AddToLink(BaseAST *childe);

        void DFS();
    };
}
#endif //COMPILE_AST_H
