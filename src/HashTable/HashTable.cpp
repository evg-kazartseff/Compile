//
// Created by evgenii on 06.03.18.
//

#include "../../inc/HashTable.h"

HashTable::HashTable() {
    this->table = new Table();
    if (!table) {
        std::cerr << "Error malloc HashTable" << std::endl;
    }
}

void HashTable::CreateEntry(int type, std::string value)
{
    if (!table) {
        std::cerr << "HashTable is Null" << std::endl;
    }
    this->table->CreateEntry(type, std::move(value));
}

HashNode *HashTable::LookupEntry(const std::string &value)
{
    if (!table) {
        std::cerr << "HashTable is Null" << std::endl;
        return nullptr;
    }
    return this->table->LookupEntry(value);
}

HashNode *HashTable::LookupEntryNotRecur(const std::string &value)
{
    if (!table) {
        std::cerr << "HashTable is Null" << std::endl;
        return nullptr;
    }
    return this->table->LookupEntryNotRecur(value);
}

void HashTable::addChildScope()
{
    auto *newScope = new Table();

    this->table->addChildScope(newScope);
    this->table = newScope;
}

void HashTable::closeScope()
{
    if (!table) {
        std::cerr << "HashTable is Null" << std::endl;
    }
    Table *parent = this->table->getParent();
    if (!parent) {
        std::cerr << "Parent HashTable is Null" << std::endl;
    }

    this->table = parent;
}

void HashTable::getChlidScope()
{
    if (!table) {
        std::cerr << "HashTable is Null" << std::endl;
    }
    Table *tmp_table = this->table->getChlidScope();
    if (!table) {
        std::cerr << "Child HashTable is Null" << std::endl;
    }

    this->table = tmp_table;
}

void HashTable::deleteThisScope()
{
    if (!table) {
        std::cerr << "Table not exist" << std::endl;
        return;
    }
    Table *parent = this->table->getParent();
    if (parent)
        parent->popChildScope();
    delete this->table;
    this->table = parent;
}

void HashTable::setAddr(const std::string &name, int addr)
{
    if (!table) {
        std::cerr << "Table not exist" << std::endl;
        return;
    }
    this->table->setAddr(name, addr);
}

int HashTable::getAddr(const std::string &name)
{
    if (!table) {
        std::cerr << "Table not exist" << std::endl;
        return -1;
    }
    return this->table->getAddr(name);
}

HashTable::~HashTable()
{
    if (this->table) {
        delete table;
        table = nullptr;
    }
}
