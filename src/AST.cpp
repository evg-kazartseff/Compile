//
// Created by evgenii on 31.03.18.
//

#include "AST.h"
#include <utility>

AST::AST::AST() {

}

void AST::AST::EndParse(BaseAST* tree) {
    this->tree = tree;
}

AST::BaseAST* AST::AST::GetIntNumberExpr(int type, int val) {
    return new IntNumberExprAST(type, val);
}

AST::BaseAST* AST::AST::GetDoubleNumberExpr(int type, double val) {
    return new DoubleNumberExprAST(type, val);
}

AST::BaseAST* AST::AST::GetVariableExpr(int type, std::string name) {
    return new VariableExprAST(type, std::move(name));
}

AST::BaseAST* AST::AST::GetBinaryExpr(int type, char op, BaseAST* lhs, BaseAST* rhs) {
    return new BinaryExprAST(type, op, lhs, rhs);
}

void AST::LinkAST::AddChild(BaseAST* child) {
    if (!this->Childs) this->Childs = new std::vector<BaseAST*>();
    this->Childs->push_back(child);
}

double AST::AST::ParseBinaryExpr(BaseAST* Expr) {
    BinaryExprAST* bin = nullptr;
    if ((bin = dynamic_cast<BinaryExprAST*>(Expr))) {
        IntNumberExprAST* Int;
        DoubleNumberExprAST* Double;
        switch (bin->LHS->Type) {
            case typeIvar:
                Int = dynamic_cast<IntNumberExprAST*>(bin->LHS);
                break;
            case typeDvar:
                Double = dynamic_cast<DoubleNumberExprAST*>(bin->LHS);
                break;
            default:
                std::cerr << "runtime error" << std::endl;
                break;
        }
        switch (bin->RHS->Type) {
            case typeIvar:
                Int = dynamic_cast<IntNumberExprAST*>(bin->RHS);
                break;
            case typeDvar:
                Double = dynamic_cast<DoubleNumberExprAST*>(bin->RHS);
                break;
            default:
                std::cerr << "runtime error" << std::endl;
                break;
        }
        if (bin->Op == '+') {
            return Int->Val + Double->Val;
        }
    }
    else {
        std::cerr << "runtime error" << std::endl;
    }
    return 0.0;
}
