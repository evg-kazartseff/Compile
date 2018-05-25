//
// Created by direnol on 20.05.18.
//
#include "../../inc/Ast.h"

std::string AST::VariableExprAST::Generate_code()
{
    int addr = hashTable->getAddr(this->Name);
    std::string str = "-" + std::to_string(addr) + "(%ebp)";
    if (this->needed) {
        str = "\tleal " + str + ", %eax\n";
        str += "\tpushl %eax\n";
    } else {
        str = "\tpushl " + str + "\n";
    }
    asmVars->IncStack(INT_SIZE);
    return str;
}

void AST::VariableExprAST::Dfs()
{
    if (!needed)
        write_adapter->Print("\t#Variable start\n");
    else
        write_adapter->Print("\t#Unary & start\n");

//    std::cout << ' ' << this->Name << ' ';
    this->write_adapter->Print(this->Generate_code());

    if (!needed)
        write_adapter->Print("\t#Variable end\n");
    else
        write_adapter->Print("\t#Unary & end\n");
}

std::string AST::IntNumberExprAST::Generate_code()
{
    std::string str = "\tpushl $" + std::to_string(Val) + "\n";
    asmVars->IncStack(INT_SIZE);
    return str;
}

void AST::IntNumberExprAST::Dfs()
{
    write_adapter->Print("\t#Constant int start\n");
//    std::cout << ' ' << this->Val << ' ';
    this->write_adapter->Print(this->Generate_code());
    write_adapter->Print("\t#Constant int end\n");
}

std::string AST::DoubleNumberExprAST::Generate_code()
{
    std::string str; //= "\tpushl $" + std::to_string(Val) + "\n";
    return str;
}

void AST::DoubleNumberExprAST::Dfs()
{
    write_adapter->Print("\t#Const double start\n");
//    std::cout << ' ' << this->Val << ' ';
    this->write_adapter->Print(this->Generate_code());
    write_adapter->Print("\t#Const double end\n");
}

std::string AST::VariableUndefAST::Generate_code()
{
    int size = this->Type == this->asmVars->getIntType() ? INT_SIZE : 8;
    this->asmVars->IncStack(size);
    int pos = this->asmVars->getStack();
    this->hashTable->setAddr(this->Name, pos);
    std::string code = "\n\tsubl $" + std::to_string(size) + ", %esp\n";
    code += "\tmovl $0, -" + std::to_string(pos) + "(%ebp)\n";
    return code;
}

void AST::VariableUndefAST::Dfs()
{
    write_adapter->Print("\t#Variable undefined " + Name + " start \n");
//    std::cout << "Undefvar " << this->Name.c_str() << std::endl;
    this->write_adapter->Print(VariableUndefAST::Generate_code());
    write_adapter->Print("\t#Variable undefined " + Name + " end\n");
}

std::string AST::VariableDefAST::Generate_code()
{
    int shift = this->hashTable->getAddr(this->Name);
    std::string pos = "-" + std::to_string(shift) + "(%ebp)\n";
    std::string str = "\tpopl %eax\n"
                      "\tmovl %eax, " + pos;
    asmVars->DecStack(INT_SIZE);
    return str;
}

void AST::VariableDefAST::Dfs()
{
    write_adapter->Print("\t#Variable defined start\n");
    VariableUndefAST::Dfs();
    this->Expr->Dfs();
    this->write_adapter->Print(Generate_code());
    write_adapter->Print("\t#Variable defined end\n");
}

AST::VariableDefAST::~VariableDefAST()
{
    if (this->Expr) {
        delete (this->Expr);
        this->Expr = nullptr;
    }
}
