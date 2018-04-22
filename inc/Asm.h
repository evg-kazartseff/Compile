//
// Created by evgenii on 21.04.18.
//

#ifndef COMPILE_ASM_H
#define COMPILE_ASM_H

#include <fstream>

class ASM_GEN {
private:
    std::fstream file;
public:
    ASM_GEN(std::string filename);

};

#endif //COMPILE_ASM_H
