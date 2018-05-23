//
// Created by evgenii on 20.05.18.
//

#include "../../inc/asm/ASM_VARS.h"

void AsmVars::setIntType(size_t var)
{
    this->var_size.Int = var;
}

size_t AsmVars::getIntType()
{
    return this->var_size.Int;
}

void AsmVars::setStack(int new_stack)
{
    this->stack = new_stack;
}

int AsmVars::getStack()
{
    return this->stack;
}

void AsmVars::IncStack(int size)
{
    this->stack += size;
}

void AsmVars::DecStack(int size)
{
    this->stack -= size;
}

void AsmVars::setAddrResult(int AddrResult)
{
    this->addr_result = AddrResult;
}

int AsmVars::getAddrResult()
{
    return this->addr_result;
}
