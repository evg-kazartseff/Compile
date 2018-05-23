//
// Created by direnol on 23.05.18.
//

#include "../../inc/HashTable/HashEntry.h"

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

HashEntry::~HashEntry()
{
    HashNode *node = this->list;
    if (node) {
        delete node->getNext();
        delete node;
    }
}

HashEntry::HashEntry()
{
    this->list = nullptr;
}
