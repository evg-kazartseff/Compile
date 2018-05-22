//
// Created by evgenii on 06.03.18.
//

#ifndef LAB2_HASHTABLE_H
#define LAB2_HASHTABLE_H

#include <string>
#include <iostream>
#include <queue>
#include <utility>

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

class Table {
    friend Ast;
private:
    size_t size;
    HashEntry *table;
    Table *parent;
    std::queue<Table *> childs;
    size_t GetHash(const char *key);
    size_t getSize();
public:
    explicit Table(size_t size = 128);
    ~Table();
    void CreateEntry(int type, std::string value);
    HashNode *LookupEntry(const std::string &value);
    HashNode *LookupEntryNotRecur(const std::string &value);
    void DeleteEntry(const std::string &value);

    void setParent(Table *table);
    Table *getParent();

    void addChildScope(Table *scope);
    Table *getChlidScope();
    void popChildScope();
    void deleteAllChildScope();

    void setAddr(const std::string& name, int addr);
    int getAddr(const std::string& name);
};

class HashTable {
    Table* table;
public:
    HashTable();
    void CreateEntry(int type, std::string value) {
        this->table->CreateEntry(type, std::move(value));
    }

    HashNode *LookupEntry(const std::string &value) {
        return this->table->LookupEntry(value);
    }

    HashNode *LookupEntryNotRecur(const std::string &value)
    {
        return this->table->LookupEntryNotRecur(value);
    }

    void DeleteEntry(const std::string &value) {
        this->table->DeleteEntry(value);
    }

    void setParent(Table *table) {
        this->table->setParent(table);
    }

    Table *getParent() {
        return this->table->getParent();
    }

    void addChildScope() {
        Table* newScope = new Table();
        this->table->addChildScope(newScope);
        this->table = newScope;
    }

    void closeScope() {
        Table* parent = this->table->getParent();
        this->table = parent;
    }

    void getChlidScope()
    {
        Table *tmp_table = this->table->getChlidScope();
        this->table = tmp_table;
    }

    void popChildScope() {
       this->table->popChildScope();
    }

    void deleteAllChildScope() {
        this->table->deleteAllChildScope();
    }

    void deleteThisScope() {
        Table* parent = this->table->getParent();
        if (parent)
            parent->popChildScope();
        delete this->table;
        this->table = parent;
    }

    void setAddr(const std::string& name, int addr) {
        this->table->setAddr(name, addr);
    }

    int getAddr(const std::string& name) {
        return this->table->getAddr(name);
    }
};

#endif //LAB2_HASHTABLE_H
