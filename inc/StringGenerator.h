//
// Created by evgenii on 20.05.18.
//

#ifndef COMPILE_STRINGNAMEGENERATOR_H
#define COMPILE_STRINGNAMEGENERATOR_H

#include <cstddef>
#include <string>
#include <vector>

class StringGenerator {
private:
    size_t inc;
protected:
    std::string prefix;
public:
    StringGenerator();
    ~StringGenerator() = default;
    std::string Generate();
};

class MarkGenerator : public StringGenerator {
public:
    MarkGenerator();
};

class FormatAcum {
private:
    std::vector<std::pair<std::string, std::string>> Acum;
public:
    FormatAcum() = default;
    ~FormatAcum() = default;
    void Add(std::string str_name, std::string format);
    std::vector<std::pair<std::string, std::string>> GetAcum();
};

#endif //COMPILE_STRINGNAMEGENERATOR_H
