//
// Created by evgenii on 06.03.18.
//

#ifndef LAB2_HASHTABLE_H
#define LAB2_HASHTABLE_H

#include <string>
#include <queue>

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

class HashEntry {
private:
    HashNode *list;

    void DeleteNode(HashNode *node);

public:
    HashEntry() = default;

    ~HashEntry();

    void AddValueNode(int type, std::string value);

    HashNode *LookupValueNode(const std::string &value);

    void DeleteValueNode(const std::string &value);
};

class HashTable {
    friend Ast;
private:
    size_t size;
    HashEntry *table;
    HashTable *parent;
    std::queue<HashTable *> childs;
    size_t GetHash(const char *key);
    size_t getSize();
public:
    explicit HashTable(size_t size = 128);
    ~HashTable();
    void CreateEntry(int type, std::string value);
    HashNode *LookupEntry(const std::string &value);
    void DeleteEntry(const std::string &value);

    void setParent(HashTable *table);
    HashTable *getParent();

    void addChildScope();
    void addChildScope(HashTable *scope);
    HashTable *getChlidScope();
    void popChildScope();
    void deleteAllChildScope();

    void setAddr(const std::string& name, int addr);
    int getAddr(const std::string& name);
};

#endif //LAB2_HASHTABLE_H
