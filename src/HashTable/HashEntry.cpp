//
// Created by direnol on 23.05.18.
//

#include "../../inc/HashEntry.h"

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
