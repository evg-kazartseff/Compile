//
// Created by evgenii on 31.03.18.
//

#include "../inc/Ast.h"
#include <utility>
#include <typeindex>
#include <search.h>

AST::Ast::Ast() {}

void AST::Ast::EndParse(BaseAST* tree) {
    this->tree = tree;
}

AST::BaseAST* AST::Ast::GetIntNumberExpr(int type, int val) {
    return new IntNumberExprAST(type, val);
}

AST::BaseAST* AST::Ast::GetDoubleNumberExpr(int type, double val) {
    return new DoubleNumberExprAST(type, val);
}

AST::BaseAST* AST::Ast::GetVariableExpr(int type, std::string name) {
    return new VariableExprAST(type, std::move(name));
}

AST::BaseAST* AST::Ast::GetBinaryExpr(int type, char op, BaseAST* lhs, BaseAST* rhs) {
    return new BinaryExprAST(type, op, lhs, rhs);
}

void AST::LinkAST::AddChild(BaseAST* child) {
    if (!this->Childs) this->Childs = new std::vector<BaseAST*>();
    this->Childs->push_back(child);
}

// TODO Add parse variable as 'var', 'tmp'
AST::BaseAST* AST::Ast::ParseBinaryExpr(BaseAST* Expr) {
    BinaryExprAST* bin = nullptr;
    if ((bin = dynamic_cast<BinaryExprAST*>(Expr))) {
        IntNumberExprAST* IntOne;
        IntNumberExprAST* IntTwo;
        DoubleNumberExprAST* DoubleOne;
        DoubleNumberExprAST* DoubleTwo;
        int ret_pr = 0;
        switch (bin->LHS->Type) {
            case typeIvar:
                IntOne = dynamic_cast<IntNumberExprAST*>(bin->LHS);
                ret_pr = 0;
                break;
            case typeDvar:
                DoubleOne = dynamic_cast<DoubleNumberExprAST*>(bin->LHS);
                ret_pr = 2;
                break;
            default:
                std::cerr << "runtime error" << std::endl;
                break;
        }
        switch (bin->RHS->Type) {
            case typeIvar:
                IntTwo = dynamic_cast<IntNumberExprAST*>(bin->RHS);
                ret_pr += 1;
                break;
            case typeDvar:
                DoubleTwo = dynamic_cast<DoubleNumberExprAST*>(bin->RHS);
                ret_pr += 2;
                break;
            default:
                std::cerr << "runtime error" << std::endl;
                break;
        }
        switch (bin->Op) {
            case '+':
                switch (ret_pr) {
                    case 1:
                        return new IntNumberExprAST(nodeEnum::typeIvar, IntOne->Val + IntTwo->Val);
                    case 2:
                        return new DoubleNumberExprAST(nodeEnum::typeDvar, IntOne->Val + DoubleTwo->Val);
                    case 3:
                        return new DoubleNumberExprAST(nodeEnum::typeDvar, DoubleOne->Val + IntTwo->Val);
                    case 4:
                        return new DoubleNumberExprAST(nodeEnum::typeDvar, DoubleOne->Val + DoubleTwo->Val);
                    default:
                        break;
                }
                break;
            case '-':
                switch (ret_pr) {
                    case 1:
                        return new IntNumberExprAST(nodeEnum::typeIvar, IntOne->Val - IntTwo->Val);
                    case 2:
                        return new DoubleNumberExprAST(nodeEnum::typeDvar, IntOne->Val - DoubleTwo->Val);
                    case 3:
                        return new DoubleNumberExprAST(nodeEnum::typeDvar, DoubleOne->Val - IntTwo->Val);
                    case 4:
                        return new DoubleNumberExprAST(nodeEnum::typeDvar, DoubleOne->Val - DoubleTwo->Val);
                    default:
                        break;
                }
                break;
            case '*':
                switch (ret_pr) {
                    case 1:
                        return new IntNumberExprAST(nodeEnum::typeIvar, IntOne->Val * IntTwo->Val);
                    case 2:
                        return new DoubleNumberExprAST(nodeEnum::typeDvar, IntOne->Val * DoubleTwo->Val);
                    case 3:
                        return new DoubleNumberExprAST(nodeEnum::typeDvar, DoubleOne->Val * IntTwo->Val);
                    case 4:
                        return new DoubleNumberExprAST(nodeEnum::typeDvar, DoubleOne->Val * DoubleTwo->Val);
                    default:
                        break;
                }
                break;
            case '/':
                switch (ret_pr) {
                    case 1:
                        return new IntNumberExprAST(nodeEnum::typeIvar, IntOne->Val / IntTwo->Val);
                    case 2:
                        return new DoubleNumberExprAST(nodeEnum::typeDvar, IntOne->Val / DoubleTwo->Val);
                    case 3:
                        return new DoubleNumberExprAST(nodeEnum::typeDvar, DoubleOne->Val / IntTwo->Val);
                    case 4:
                        return new DoubleNumberExprAST(nodeEnum::typeDvar, DoubleOne->Val / DoubleTwo->Val);
                    default:
                        break;
                }
                break;
        }
    }
    else {
        std::cerr << "runtime error" << std::endl;
    }
}

void AST::Ast::DFS() {
    if (this->tree == nullptr) return;
    ///Дописать обход дерева
}

std::string AST::IntNumberExprAST::Generate_code() {
    return std::__cxx11::string();
}

void AST::IntNumberExprAST::Dfs() {
    std::cout << this->Val << std::endl;
    //this->Generate_code();
}

std::string AST::DoubleNumberExprAST::Generate_code() {
    return std::__cxx11::string();
}

void AST::DoubleNumberExprAST::Dfs() {
    std::cout << this->Val << std::endl;
    //this->Generate_code();
}

std::string AST::VariableExprAST::Generate_code() {
    return std::__cxx11::string();
}

void AST::VariableExprAST::Dfs() {
    std::cout << this->Name << std::endl;
    ///this->Generate_code();
}

std::string AST::BinaryExprAST::Generate_code() {
    return std::__cxx11::string();
}

void AST::BinaryExprAST::Dfs() {
    this->LHS->Dfs();
    this->RHS->Dfs();
}

std::string AST::CallExprAST::Generate_code() {
    return std::__cxx11::string();
}

void AST::CallExprAST::Dfs() {

}
