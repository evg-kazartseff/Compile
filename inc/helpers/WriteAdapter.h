//
// Created by evgenii on 19.05.18.
//

#ifndef COMPILE_WRITEADAPTER_H
#define COMPILE_WRITEADAPTER_H

#include <fstream>


class WriteAdapter {
private:
    std::ofstream file;
public:
    WriteAdapter() = default;

    ~WriteAdapter();

    void SetFile(const std::string &file);

    void Print(const std::string &str);
};

#endif //COMPILE_WRITEADAPTER_H
