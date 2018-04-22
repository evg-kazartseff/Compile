//
// Created by evgenii on 21.04.18.
//

#include "Asm.h"

ASM_GEN::ASM_GEN(std::string filename) {
    this->file.open(filename.c_str(), std::fstream::out);
}
