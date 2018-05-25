//
// Created by direnol on 25.05.18.
//
#ifndef COMPILE_ASTBASE_H
#define COMPILE_ASTBASE_H

#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <sstream>
#include "../helpers/WriteAdapter.h"
#include "../helpers/Singleton.h"
#include "../asm/Asm_vars.h"
#include "../HashTable/HashTable.h"
#include "../helpers/StringGenerator.h"
#include <cstring>

namespace AST {
    class Ast;

/// BaseAST - Базовый класс для всех узлов выражений.
    class BaseAST {
    protected:
        WriteAdapter *write_adapter;
        AsmVars *asmVars;
        HashTable *hashTable;
        bool needed;
    public:
        BaseAST();

        virtual ~BaseAST() = default;

        virtual std::string Generate_code() = 0;

        virtual void Dfs() = 0;

        virtual void setNeed();

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
};

#endif