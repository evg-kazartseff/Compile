//
// Created by direnol on 23.05.18.
//


#include "../../inc/HashTable.h"

Table::Table(size_t size)
{
    this->size = size;
    this->parent = nullptr;
    this->table = new HashEntry[this->size];
}

void Table::CreateEntry(int type, std::string value)
{
    size_t hash = this->GetHash(value);
    this->table[hash].AddValueNode(type, std::move(value));
}

size_t Table::GetHash(const std::string &key)
{
    std::hash<std::string> hash;
    return hash(key) % size;
}

HashNode *Table::LookupEntry(const std::string &value)
{
    size_t hash = this->GetHash(value);
    HashNode* node = this->table[hash].LookupValueNode(value);
    if (!node) {
        Table* parent = this->getParent();
        if (parent)
            node = parent->LookupEntry(value);
    }
    return node;
}

HashNode* Table::LookupEntryNotRecur(const std::string &value) {
    size_t hash = this->GetHash(value);
    return this->table[hash].LookupValueNode(value);
}

Table::~Table()
{
    while (!childs.empty()) {
        delete childs.front();
        childs.pop();
    }
    if (this->table) {
        delete [] table;
        table = nullptr;
    }
}

void Table::setParent(Table *table)
{
    this->parent = table;
}

void Table::addChildScope(Table *scope)
{
    scope->setParent(this);
    this->childs.push(scope);
}

Table *Table::getChlidScope()
{
    return this->childs.front();
}

void Table::popChildScope()
{
    if (!this->childs.empty())
        this->childs.pop();
}

Table* Table::getParent()
{
    return this->parent;
}

void Table::setAddr(const std::string& name, int addr) {
    HashNode* node = this->LookupEntry(name);
    if(!node) {
        std::cerr << "Undefined variable: " << name << std::endl;
        return;
    }
    node->setAddr(addr);
}

int Table::getAddr(const std::string &name) {
    HashNode* node = this->LookupEntry(name);
    if(!node) {
        std::cerr << "Undefined variable: " << name << std::endl;
        return -1;
    }
    return node->getAddr();
}

