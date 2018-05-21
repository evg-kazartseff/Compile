//
// Created by direnol on 20.05.18.
//
#include "../../inc/Ast.h"
//#include "../../inc/parser.h"

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
        case '&':
            operation = "\tandl %ebx, %eax\n";
            break;
        case '^':
            operation = "\txorl %ebx, %eax\n";
            break;
        case '|':
            operation = "\torl %ebx, %eax\n";
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
    if (this->need_ret) {
        str += "\tpushl %eax\n";
        asmVars->IncStack(INT_SIZE);
    }
    return str;
}

void AST::EvalAST::Dfs()
{
    this->Expr->Dfs();
    write_adapter->Print(Generate_code());
}

void AST::EvalAST::SetNeed()
{
    this->need_ret = true;
}

std::string AST::UnaryAST::Generate_code()
{
    std::string mtrue, mend;
    std::string str = "\tpopl %eax\n";
    switch (this->Operation) {
        case '~':
            str += "\tnot %eax\n";
            break;
        case '-':
            str += "\tnegl %eax\n";
            break;
        case oINC:
            str += "\tpushl (%eax)\n"
                   "\tincl (%eax)\n"
                   "\tpopl %eax\n";
            break;
        case oIINC:
            str += "\tincl (%eax)\n"
                   "\tmovl (%eax), %ebx\n"
                   "\tmovl %ebx, %eax\n";
            break;
        case oDEC:
            str += "\tpushl (%eax)\n"
                   "\tdecl (%eax)\n"
                   "\tpopl %eax\n";
            break;
        case oIDEC:
            str += "\tdecl (%eax)\n"
                   "\tmovl (%eax), %ebx\n"
                   "\tmovl %ebx, %eax\n";
            break;
        case '!':
            mtrue = Singleton<MarkGenerator>::getInstance()->Generate();
            mend = Singleton<MarkGenerator>::getInstance()->Generate();
            str += "\tmovl $0, %ebx\n"
                   "\tcmpl %ebx, %eax\n"
                   "\tje " + mtrue + "\n";
            str += "\tmovl $0, %eax\n"
                   "\tjmp " + mend + "\n";
            str += mtrue + ":\n";
            str += "\tmovl $1, %eax\n" +
                   mend + ":\n";
            break;
        default:
            break;
    }
    str += "\tpushl %eax\n";
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
                      "\tcmpl %ebx, %eax\n";

    std::string mfalse = Singleton<MarkGenerator>::getInstance()->Generate();
    std::string mend = Singleton<MarkGenerator>::getInstance()->Generate();

    asmVars->DecStack(INT_SIZE * 2);
    std::string operation;
    switch (this->Op) {
        case '<':
            operation = "\tjns ";
            break;
        case '>':
            operation = "\tjle ";
            break;
        case '=':
            operation = "\tjne ";
            break;
        case oLEQ:
            operation = "\tjg ";
            break;
        case oGEQ:
            operation = "\tjs ";
            break;
        case '!':
            operation = "\tje ";
            break;
        case '&':
            write_adapter->Print("\tcmpl $0, %eax\n");
            write_adapter->Print("\tje " + mfalse + "\n");
            write_adapter->Print("\tpopl %eax\n");
            write_adapter->Print("\tcmpl $0, %eax\n");
            write_adapter->Print("\tje " + mfalse + "\n");
            break;
        default:
            break;
    }
    if (this->Op != '&')
        str += operation + mfalse + "\n";
    str += "\tpushl $1\n" // false
           "\tjmp " + mend + "\n";

    str += mfalse + ":\n"; // true
    str += "\tpushl $0\n" +
           mend + ":\n";

    asmVars->IncStack(INT_SIZE);
    return str;
}

void AST::LogicExprAST::Dfs()
{
    this->LHS->Dfs();
    this->RHS->Dfs();
    write_adapter->Print(Generate_code());
}