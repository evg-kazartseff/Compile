//
// Created by direnol on 20.05.18.
//
#include "../../inc/Ast.h"

std::string AST::VariableExprAST::Generate_code()
{
    int addr = hashTable->getAddr(this->Name);
    std::string str = "-" + std::to_string(addr) + "(%ebp)";
    if (this->Is_addr) {
        str = "\tleal " + str + ", %eax\n";
        str += "\tpushl %eax\n";
    } else {
        str = "\tpushl " + str + "\n";
    }

    return str;
}

void AST::VariableExprAST::Dfs()
{
//    std::cout << ' ' << this->Name << ' ';
    this->write_adapter->Print(this->Generate_code());
}

std::string AST::IntNumberExprAST::Generate_code()
{
    std::string str = "\tpushl $" + std::to_string(Val) + "\n";
    return str;
}

void AST::IntNumberExprAST::Dfs()
{
//    std::cout << ' ' << this->Val << ' ';
    this->write_adapter->Print(this->Generate_code());
}

std::string AST::DoubleNumberExprAST::Generate_code()
{
    std::string str; //= "\tpushl $" + std::to_string(Val) + "\n";
    return str;
}

void AST::DoubleNumberExprAST::Dfs()
{
//    std::cout << ' ' << this->Val << ' ';
    this->write_adapter->Print(this->Generate_code());
}


std::string AST::VariableUndefAST::Generate_code()
{
    int size = this->Type == this->asmVars->getIntType() ? 4 : 8;
    this->asmVars->IncStack(size);
    int pos = this->asmVars->getStack();
    this->hashTable->setAddr(this->Name, pos);
    std::string code = "\tsubl $" + std::to_string(size) + ", %esp\n";
    code += "\tmovl $0, -" + std::to_string(pos) + "(%ebp)\n";
    return code;
}

void AST::VariableUndefAST::Dfs()
{
//    std::cout << "Undefvar " << this->Name.c_str() << std::endl;
    this->write_adapter->Print(VariableUndefAST::Generate_code());
}

std::string AST::VariableDefAST::Generate_code()
{
    int shift = this->hashTable->getAddr(this->Name);
    std::string pos = "-" + std::to_string(shift) + "(%ebp)\n";
    std::string str = "\tpopl %eax\n"
                      "\tmovl %eax, " + pos;
    return str;
}

void AST::VariableDefAST::Dfs()
{
    VariableUndefAST::Dfs();
    this->Expr->Dfs();
    this->write_adapter->Print(Generate_code());
}