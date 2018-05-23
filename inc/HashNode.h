//
// Created by direnol on 24.05.18.
//

#ifndef COMPILE_HASHNODE_H
#define COMPILE_HASHNODE_H

#include <string>

class Ast;


class HashNode {
private:
    std::string value;
    int type;
    int addr;
    HashNode *next;
    HashNode *prev;
public:
    explicit HashNode(int type, std::string &Value);

    ~HashNode();

    void setNext(HashNode *next);

    void setPrev(HashNode *prev);

    HashNode *getNext() const;

    HashNode *getPrev() const;

    std::string &getValue();

    int getType();

    void setValue(std::string &value);

    void setType(int type);

    void setAddr(int addr);

    int getAddr();
};

#endif //COMPILE_HASHNODE_H
