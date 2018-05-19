//
// Created by evgenii on 06.03.18.
//

#include <utility>
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

HashTable::HashTable(size_t size)
{
    this->size = size;
    this->parent = nullptr;
    this->table = new HashEntry[this->size];
}

void HashTable::CreateEntry(int type, std::string value)
{
    size_t hash = this->GetHash(value.c_str());
    this->table[hash].AddValueNode(type, std::move(value));
}

size_t HashTable::GetHash(const char *key)
{
    unsigned long hash = 5381;
    int c;

    while ((c = *key++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash % this->size;
}

HashNode *HashTable::LookupEntry(const std::string &value)
{
    size_t hash = this->GetHash(value.c_str());
    return this->table[hash].LookupValueNode(value);
}

void HashTable::DeleteEntry(const std::string &value)
{
    size_t hash = this->GetHash(value.c_str());
    this->table[hash].DeleteValueNode(value);
}

HashTable::~HashTable()
{
}

void HashTable::addChildScope()
{
    HashTable *NewScope = new HashTable(this->getSize());
    this->addChildScope(NewScope);
}

void HashTable::setParent(HashTable *table)
{
    this->parent = table;
}

size_t HashTable::getSize()
{
    return this->size;
}

void HashTable::addChildScope(HashTable *scope)
{
    scope->setParent(this);
    this->childs.push(scope);
}

HashTable *HashTable::getChlidScope()
{
    return this->childs.front();
}

void HashTable::popChildScope()
{
    if (!this->childs.empty())
        this->childs.pop();
}

void HashTable::deleteAllChildScope()
{
    while (!this->childs.empty())
        this->childs.pop();
}

HashTable *HashTable::getParent()
{
    return this->parent;
}

HashEntry *HashTable::getTable()
{
    return this->table;
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
