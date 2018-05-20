//
// Created by evgenii on 20.05.18.
//

#ifndef COMPILE_STRINGNAMEGENERATOR_H
#define COMPILE_STRINGNAMEGENERATOR_H

#include <cstddef>
#include <string>

class StringNameGenerator {
private:
    size_t inc;
    std::string prefix;
public:
    StringNameGenerator();
    ~StringNameGenerator() = default;
    std::string Generate();
};

#endif //COMPILE_STRINGNAMEGENERATOR_H
