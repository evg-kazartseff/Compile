//
// Created by evgenii on 20.05.18.
//

#include "../../inc/StringNameGenerator.h"

StringNameGenerator::StringNameGenerator() {
    this->prefix = "LLE";
    this->inc = 0;
}

std::string StringNameGenerator::Generate() {
    return this->prefix + std::to_string(this->inc++);
}
