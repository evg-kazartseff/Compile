//
// Created by evgenii on 06.03.18.
//

#include "../inc/HashTable.h"

void HashEntry::AddValueNode(int type, std::string value)
{
    HashNode *look = this->LookupValueNode(value);
    if (look) {
        look->setValue(value);
        look->setType(type);
        return;
    }
    HashNode *node = this->list;
    HashNode *newnode;
    newnode = new HashNode(type, value);
    if (node) {
        this->list = newnode;
        newnode->setNext(node);
        node->setPrev(newnode);
    } else {
        this->list = newnode;
    }
}

HashNode *HashEntry::LookupValueNode(const std::string &value)
{
    HashNode *node = this->list;
    if (!node)
        return nullptr;
    if (node->getValue() == value)
        return node;
    while (node->getNext()) {
        node = node->getNext();
        if (node->getValue() == value)
            return node;
    }
    return nullptr;
}

void HashEntry::DeleteValueNode(const std::string &value)
{
    HashNode *node = this->LookupValueNode(value);
    if (!node)
        return;
    this->DeleteNode(node);
}

void HashEntry::DeleteNode(HashNode *node)
{
    if (!this->list)
        return;
    if (!node->getPrev()) {
        this->list = node->getNext();
    } else if (!node->getNext()) {
        node->getPrev()->setNext(nullptr);
    } else {
        node->getPrev()->setNext(node->getNext());
        node->getNext()->setPrev(node->getPrev());
    }
    delete node;
}

HashEntry::~HashEntry()
{
    HashNode *node = this->list;
    HashNode *next;
    while (node) {
        next = node->getNext();
        delete node;
        node = next;
    }
}

Table::Table(size_t size)
{
    this->size = size;
    this->parent = nullptr;
    this->table = new HashEntry[this->size];
}

void Table::CreateEntry(int type, std::string value)
{
    size_t hash = this->GetHash(value.c_str());
    this->table[hash].AddValueNode(type, std::move(value));
}

size_t Table::GetHash(const char *key)
{
    unsigned long hash = 5381;
    int c;

    while ((c = *key++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash % this->size;
}

HashNode *Table::LookupEntry(const std::string &value)
{
    size_t hash = this->GetHash(value.c_str());
    HashNode* node = this->table[hash].LookupValueNode(value);
    if (!node) {
        Table* parent = this->getParent();
        if (parent)
           node = parent->LookupEntry(value);
    }
    return node;
}

HashNode* Table::LookupEntryNotRecur(const std::string &value) {
    size_t hash = this->GetHash(value.c_str());
    return this->table[hash].LookupValueNode(value);
}

void Table::DeleteEntry(const std::string &value)
{
    size_t hash = this->GetHash(value.c_str());
    this->table[hash].DeleteValueNode(value);
}

Table::~Table()
{
}

void Table::setParent(Table *table)
{
    this->parent = table;
}

size_t Table::getSize()
{
    return this->size;
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

void Table::deleteAllChildScope()
{
    while (!this->childs.empty())
        this->childs.pop();
}

Table* Table::getParent()
{
    return this->parent;
}

void Table::setAddr(const std::string& name, int addr) {
    HashNode* node = this->LookupEntry(name);
    if(!node) {
        std::cerr << "Undefine vareable: " << name << std::endl;
        return;
    }
    node->setAddr(addr);
}

int Table::getAddr(const std::string &name) {
    HashNode* node = this->LookupEntry(name);
    if(!node) {
        std::cerr << "Undefine vareable: " << name << std::endl;
        return -1;
    }
    return node->getAddr();
}

HashNode::HashNode(int type, std::string &Value)
{
    this->type = type;
    this->value = Value;
}

HashNode::~HashNode()
{
    this->value.clear();
}

HashNode *HashNode::getNext() const
{
    return this->next;
}

void HashNode::setNext(HashNode *next)
{
    this->next = next;
}

HashNode *HashNode::getPrev() const
{
    return this->prev;
}

void HashNode::setPrev(HashNode *prev)
{
    this->prev = prev;
}

std::string &HashNode::getValue()
{
    return this->value;
}

void HashNode::setValue(std::string &value)
{
    this->value = value;
}

int HashNode::getType()
{
    return this->type;
}

void HashNode::setType(int type)
{
    this->type = type;
}

void HashNode::setAddr(int addr) {
    this->addr = addr;
}

int HashNode::getAddr() {
    return this->addr;
}

HashTable::HashTable() {
    this->table = new Table();
}
