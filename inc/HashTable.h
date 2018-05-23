//
// Created by evgenii on 06.03.18.
//

#ifndef LAB2_HASHTABLE_H
#define LAB2_HASHTABLE_H

#include <string>
#include <iostream>
#include <queue>
#include <utility>
#include "HashEntry.h"

class Table {
    friend Ast;
private:
    size_t size;
    HashEntry *table;
    Table *parent;
    std::queue<Table *> childs;

    size_t GetHash(const std::string &key);

public:
    explicit Table(size_t size = 128);

    ~Table();

    void CreateEntry(int type, std::string value);

    HashNode *LookupEntry(const std::string &value);

    HashNode *LookupEntryNotRecur(const std::string &value);

    void setParent(Table *table);

    Table *getParent();

    void addChildScope(Table *scope);

    Table *getChlidScope();

    void popChildScope();

    void setAddr(const std::string &name, int addr);

    int getAddr(const std::string &name);
};

class HashTable {
    Table *table;
public:

    HashTable();

    ~HashTable();

    void CreateEntry(int type, std::string value);

    HashNode *LookupEntry(const std::string &value);

    HashNode *LookupEntryNotRecur(const std::string &value);

    void addChildScope();

    void closeScope();

    void getChlidScope();

    void deleteThisScope();

    void setAddr(const std::string &name, int addr);

    int getAddr(const std::string &name);
};

#endif //LAB2_HASHTABLE_H
