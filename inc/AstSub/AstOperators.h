//
// Created by direnol on 25.05.18.
//

#ifndef COMPILE_ASTOPERATORS_H
#define COMPILE_ASTOPERATORS_H

#include "AstBase.h"

namespace AST {
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
};
#endif //COMPILE_ASTOPERATORS_H
