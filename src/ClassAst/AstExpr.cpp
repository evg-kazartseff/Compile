//
// Created by direnol on 20.05.18.
//

#include "../../inc/Ast.h"

std::string AST::BinaryExprAST::Generate_code()
{
    std::string str = "\tpopl %ebx\n"
                      "\tpopl %eax\n";
    std::string operation;
    switch (this->Op) {
        case '+':
            operation = "\taddl %ebx, %eax\n";
            break;
        case '*':
            operation = "\tmull %ebx\n";
            break;
        case '-':
            operation = "\tsubl %ebx, %eax\n";
            break;
        case '/':
            operation = "\tmovl $0, %edx\n"
                        "\tidiv %ebx\n";
            break;
        default:
            break;
    }
    str += operation + "\tpushl %eax\n";
    return str;
}

void AST::BinaryExprAST::Dfs()
{
    this->LHS->Dfs();
    this->RHS->Dfs();
    write_adapter->Print(Generate_code());
}

std::string AST::EvalAST::Generate_code()
{
    int addr = hashTable->getAddr(this->Id);
    std::string pos = "-" + std::to_string(addr) + "(%ebp)\n";
    std::string str = "\tpopl %eax\n"
                      "\tmovl %eax, " + pos;
    return str;
}

void AST::EvalAST::Dfs()
{
    this->Expr->Dfs();
    write_adapter->Print(Generate_code());
}

std::string AST::UnaryAST::Generate_code()
{
    return std::__cxx11::string();
}

void AST::UnaryAST::Dfs()
{
    std::cout << "(" << this->Operation;
    this->Operand->Dfs();
    std::cout << ")";
}

std::string AST::LogicExprAST::Generate_code()
{
    return std::__cxx11::string();
}

void AST::LogicExprAST::Dfs()
{
    std::cout << this->Op << " ";
    this->LHS->Dfs();
    this->RHS->Dfs();
}