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

void FormatAcum::Add(std::string str_name, std::string format) {
    this->Acum.push_back({str_name, format});
}

std::vector<std::pair<std::string, std::string>> FormatAcum::GetAcum() {
    return this->Acum;
}
