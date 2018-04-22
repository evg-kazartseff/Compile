//
// Created by evgenii on 06.03.18.
//

#include "HashTable.h"

void HashEntry::AddValueNode(std::string value) {
    HashNode* look = this->LookupValueNode(value);
    if (look) {
        look->setValue(value);
        return;
    }
    HashNode* node = this->list;
    HashNode* newnode;
    newnode = new HashNode(value);
    if (node) {
        this->list = newnode;
        newnode->setNext(node);
        node->setPrev(newnode);
    }
    else {
        this->list = newnode;
    }
}

HashNode* HashEntry::LookupValueNode(const std::string& value) {
    HashNode* node = this->list;
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

void HashEntry::DeleteValueNode(const std::string &value) {
    HashNode* node = this->LookupValueNode(value);
    if (!node)
        return;
    this->DeleteNode(node);
}

void HashEntry::DeleteNode(HashNode* node) {
    if (!this->list)
        return;
    if (!node->getPrev()) {
        this->list = node->getNext();
    }
    else if (!node->getNext()) {
        node->getPrev()->setNext(nullptr);
    }
    else {
        node->getPrev()->setNext(node->getNext());
        node->getNext()->setPrev(node->getPrev());
    }
    delete node;
}

HashEntry::~HashEntry() {
    HashNode* node = this->list;
    HashNode* next;
    while (node) {
        next = node->getNext();
        delete node;
        node = next;
    }
}

HashTable::HashTable(size_t size) {
    this->size = size;
    this->table = new HashEntry[this->size];
}

void HashTable::CreateEntry(char* value) {
    std::string val(value);
    size_t hash = this->GetHash(value);
    this->table[hash].AddValueNode(val);
}

size_t HashTable::GetHash(char* key) {
    unsigned long hash = 5381;
    int c;

    while ((c = *key++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash % this->size;
}

HashNode* HashTable::LookupEntry(char* value) {
    std::string val(value);
    size_t hash = this->GetHash(value);
    return this->table[hash].LookupValueNode(val);
}

void HashTable::DeleteEntry(char* value) {
    std::string val(value);
    size_t hash = this->GetHash(value);
    this->table[hash].DeleteValueNode(val);
}

HashTable::~HashTable() {}

HashNode::HashNode(std::string& Value) {
    this->value = Value;
}

HashNode::~HashNode() {
    this->value.clear();
}

HashNode* HashNode::getNext() const {
    return this->next;
}

void HashNode::setNext(HashNode* next) {
    this->next = next;
}

HashNode* HashNode::getPrev() const {
    return this->prev;
}

void HashNode::setPrev(HashNode* prev) {
    this->prev = prev;
}

std::string &HashNode::getValue() {
    return this->value;
}

void HashNode::setValue(std::string &value) {
    this->value = value;
}
