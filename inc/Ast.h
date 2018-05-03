//
// Created by evgenii on 31.03.18.
//

#ifndef COMPILE_AST_H
#define COMPILE_AST_H

#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <typeinfo>

namespace AST {
    typedef enum {
        typeRoot,
        typeList,
        typeCond,
        typeId,
        typeOpr,
        typeDef,
        typeArgs,
        typeFunc,
        typeTerm,
        typeBody,
        typeBodyLList,
        typeRet,
        typeIvar,
        typeDvar,
        typeCall,
        typeFargs,
        typeEval,
        typeJump,
        typeMark,
        typeLink,
        typeDefVar,
        typeUndefVar,
        typeErr
    } nodeEnum;

    class Ast;

    /// BaseAST - Базовый класс для всех узлов выражений.
    class BaseAST {
    public:
        int Type = 0;
        virtual std::string Generate_code() = 0;
        virtual void Dfs() = 0;
        BaseAST() = default;
        explicit BaseAST(int type) { this->Type = type; };
        virtual ~BaseAST() = default;
    };

    /// IntNumberExprAST - Класс узла выражения для числовых литералов (Например, "1").
    class IntNumberExprAST : public BaseAST {
    public:
        int Val;
    public:
        IntNumberExprAST(int type, int val) : BaseAST(type), Val(val) {}
        std::string Generate_code() final;
        void Dfs() final;
    };

    /// DoubleNumberExprAST - Класс узла выражения для числовых литералов (Например, "1.2").
    class DoubleNumberExprAST : public BaseAST {
    public:
        double Val;
    public:
        DoubleNumberExprAST(int type, double val) : BaseAST(type), Val(val) {}
        std::string Generate_code() final;
        void Dfs() final;
    };

    /// VariableExprAST - Класс узла выражения для переменных (например, "a").
    class VariableExprAST : public BaseAST {
        std::string Name;
    public:
        VariableExprAST(int type, std::string name) : BaseAST(type), Name(std::move(name)) {}
        std::string Generate_code() final;
        void Dfs() final;
    };

    class VariableUndefAST : public BaseAST {
        std::string Name;
    public:
        VariableUndefAST(int type, std::string name) : BaseAST(type), Name(std::move(name)) {}
        std::string Generate_code() override;
        void Dfs() override;
    };

    class VariableDefAST : public VariableUndefAST {
        BaseAST* Expr;
    public:
        VariableDefAST(int type, std::string name, BaseAST* expr)
                : VariableUndefAST(type, std::move(name)), Expr(expr) {}
        std::string Generate_code() final;
        void Dfs() final;
    };

    /// BinaryExprAST - Класс узла выражения для бинарных операторов.
    class BinaryExprAST : public BaseAST {
    public:
        char Op;
        BaseAST *LHS, *RHS;
    public:
        BinaryExprAST(int type, char op, BaseAST* lhs, BaseAST* rhs)
                : BaseAST(type), Op(op), LHS(lhs), RHS(rhs) {}
        std::string Generate_code() final;
        void Dfs() final;
    };

    /// EvalAST - Класс переопределения переменной
    class EvalAST : public BaseAST {
    public:
        std::string Id;
        BaseAST *Expr;
    public:
        EvalAST(int type, std::string id, BaseAST* expr)
                : BaseAST(type), Id(id), Expr(expr) {}
        std::string Generate_code() final;
        void Dfs() final;
    };

    class JumpAST : public BaseAST {
    public:
        std::string Id;
    public:
        JumpAST() {}
        JumpAST(int type, std::string id)
                : BaseAST(type), Id(id) {}
        std::string Generate_code() override;
        void Dfs() override;
    };

    class MarkAST : public  JumpAST {
    public:
        MarkAST(int type, std::string id)
                : JumpAST(type, id) {}
        std::string Generate_code() final;
        void Dfs() final;
    };



    /// CallExprAST - Класс узла выражения для вызова функции.
    class CallExprAST : public BaseAST {
        std::string Callee;
        std::vector<BaseAST*> Args;
    public:
        CallExprAST(std::string callee, std::vector<BaseAST*> &args)
                : Callee(std::move(callee)), Args(args) {}
        std::string Generate_code() final;
        void Dfs() final;
    };

    /// PrototypeAST - Этот класс представляет "прототип" для функции,
    /// который хранит её имя и имена аргументов (и, таким образом,
    /// неявно хранится число аргументов).
    class PrototypeAST : public BaseAST {
    public:
        std::string Name;
        BaseAST* Args = nullptr;

        PrototypeAST() = default;

        PrototypeAST(std::string name, BaseAST* args)
                : Name(std::move(name)), Args(args) {}
    };

    /// FunctionAST - Представляет определение самой функции
    class FunctionAST : public BaseAST {
        BaseAST* Prototype;
        BaseAST* Body;
    public:
        FunctionAST(BaseAST* prototype, BaseAST* body)
                : Prototype(prototype), Body(body) {}
    };

    ///LinkAST - Связующий элемент в дереве
    class LinkAST : public BaseAST {
        std::vector<BaseAST*>* Childs = nullptr;
    public:
        explicit LinkAST(int type)
                : BaseAST(type) {  this->Childs = new std::vector<BaseAST*>(); }
        explicit LinkAST(std::vector<BaseAST*>* childs)
                : Childs(childs) {}
        void AddChild(BaseAST* child);
        std::string Generate_code() final;
        void Dfs() final;
    };

    class BodyLListAST : public BaseAST {
        BaseAST* Next;
        BaseAST* Attr;
    public:
        BodyLListAST(int type, BaseAST* next, BaseAST* attr)
                : Next(next), Attr(attr), BaseAST(type) {}
        std::string Generate_code() final;
        void Dfs() final;
    };

    class BodyAST : public BaseAST {
        BaseAST* LList = nullptr;
    public:
        BodyAST(int type, BaseAST* llist)
                : BaseAST(type),  LList(llist) {}
        std::string Generate_code() final;
        void Dfs() final;
    };

    class Ast {
        LinkAST* tree;
    public:
        Ast();
        BaseAST* GetIntNumberExpr(int type, int val);
        BaseAST* GetDoubleNumberExpr(int type, double val);
        BaseAST* GetVariableExpr(int type, std::string name);
        BaseAST* GetBinaryExpr(int type, char op, BaseAST* lhs, BaseAST* rhs);
        BaseAST* GetEval(int type, std::string id, BaseAST* expr);
        BaseAST* GetJump(int type, std::string id);
        BaseAST* GetMark(int type, std::string id);
        BaseAST* GetVariableDef(int type, std::string name, BaseAST* expr);
        BaseAST* GetVariableUndef(int type, std::string name);
        BaseAST* GetBodyLList(int type, BaseAST* next, BaseAST* attr);
        BaseAST* GetBody(int type, BaseAST* llist);
        void AddToLink(int type, BaseAST* childe);
        void DFS();
    };
}
#endif //COMPILE_AST_H
