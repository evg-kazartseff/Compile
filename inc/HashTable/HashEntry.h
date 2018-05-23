//
// Created by direnol on 24.05.18.
//

#ifndef COMPILE_HASHENTRY_H
#define COMPILE_HASHENTRY_H

#include "HashNode.h"

class HashEntry {
private:
    HashNode *list;

public:
    HashEntry();

    ~HashEntry();

    void AddValueNode(int type, std::string value);

    HashNode *LookupValueNode(const std::string &value);

};

#endif //COMPILE_HASHENTRY_H
