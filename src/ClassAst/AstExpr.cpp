//
// Created by direnol on 20.05.18.
//
#include "../../inc/Ast.h"

std::string AST::BinaryExprAST::Generate_code()
{
    std::string str = "\tpopl %ebx\n"
                      "\tpopl %eax\n";
    asmVars->DecStack(INT_SIZE * 2);
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
    asmVars->IncStack(INT_SIZE);
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
    asmVars->DecStack(INT_SIZE);
    return str;
}

void AST::EvalAST::Dfs()
{
    this->Expr->Dfs();
    write_adapter->Print(Generate_code());
}

std::string AST::UnaryAST::Generate_code()
{
    std::string str = "";
    switch (this->Operation) {
        case '~':
            break;
        case '!':
            break;
        default:
            break;
    }
    return str;
}

void AST::UnaryAST::Dfs()
{
    this->Operand->Dfs();
    this->write_adapter->Print(Generate_code());
}

std::string AST::LogicExprAST::Generate_code()
{
    std::string str = "\tpopl %ebx\n"
                      "\tpopl %eax\n"
                      "\tcmp %eax, %ebx\n";

    std::string mtrue = Singleton<MarkGenerator>::getInstance()->Generate();
    std::string mend = Singleton<MarkGenerator>::getInstance()->Generate();

    asmVars->DecStack(INT_SIZE * 2);
    std::string operation;
    switch (this->Op) {
        case '<':
            operation = "\tjl ";
            break;
        case '>':
            operation = "\tja ";
            break;
        case '=':
            operation = "\tje ";
            break;
        default:
            break;
    }
    str += operation + mtrue + "\n";
    str += "\tpushl $0\n" // false
           "\tjmp " + mend + "\n";

    str += mtrue + ":\n"; // true
    str + "\tpushl $1\n" + mend + ":\n";

    asmVars->IncStack(INT_SIZE);
    return str;
}

void AST::LogicExprAST::Dfs()
{
    this->LHS->Dfs();
    this->RHS->Dfs();
    write_adapter->Print(Generate_code());
}