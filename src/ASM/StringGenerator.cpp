//
// Created by evgenii on 20.05.18.
//

#include "../../inc/StringGenerator.h"

StringGenerator::StringGenerator() {
    this->prefix = "LLE";
    this->inc = 0;
}

std::string StringGenerator::Generate() {
    return this->prefix + std::to_string(this->inc++);
}

void FormatAcum::Add(std::string str_name, std::string format) {
    this->Acum.emplace_back(str_name, format);
}

std::vector<std::pair<std::string, std::string>> FormatAcum::GetAcum() {
    return this->Acum;
}

MarkGenerator::MarkGenerator() {
    this->prefix = "Mark";
}
