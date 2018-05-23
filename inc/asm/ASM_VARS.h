//
// Created by evgenii on 20.05.18.
//

#ifndef COMPILE_ASM_VARS_H
#define COMPILE_ASM_VARS_H

#include <cstddef>

#define INT_SIZE 4

enum {
    oLEQ = -66,
    oGEQ,
    oDEC,
    oIDEC,
    oINC,
    oIINC,
};

typedef struct {
    size_t Int;
    size_t Double;
} VarType;

class AsmVars {
private:
    size_t size;
    int stack;
    int addr_result;
    VarType var_size;
public:
    AsmVars() = default;

    ~AsmVars() = default;

    void setIntType(size_t var);

    size_t getIntType();

    void setStack(int new_stack);

    int getStack();

    void IncStack(int size);

    void DecStack(int size);

    void setAddrResult(int AddrResult);

    int getAddrResult();
};

#endif //COMPILE_ASM_VARS_H
