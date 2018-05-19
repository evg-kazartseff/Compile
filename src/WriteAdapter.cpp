//
// Created by evgenii on 19.05.18.
//

#include <iostream>
#include "../inc/WriteAdapter.h"

void WriteAdapter::SetFile(const std::string& file) {
    this->file.open(file);
}

void WriteAdapter::Print(const std::string& str) {
    std::cout << str;
    if (this->file.is_open())
        this->file.write(str.c_str(), str.length());
}

WriteAdapter::~WriteAdapter() {
    this->file.flush();
    this->file.close();
}
