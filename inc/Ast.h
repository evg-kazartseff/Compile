//
// Created by evgenii on 31.03.18.
//

#ifndef COMPILE_AST_H
#define COMPILE_AST_H


#include "AstSub/AstVar.h"
#include "AstSub/AstExpr.h"
#include "AstSub/AstFunc.h"
#include "AstSub/AstOperators.h"
#include "AstSub/AstVar.h"

namespace AST {


    class Ast {
        BaseAST *tree;
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
