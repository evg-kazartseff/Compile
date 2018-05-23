//
// Created by direnol on 23.05.18.
//

#include "../../inc/HashTable/HashNode.h"

HashNode::HashNode(int type, std::string &Value)
{
    this->next = nullptr;
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

void HashNode::setPrev(HashNode *prev)
{
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

void HashNode::setAddr(int addr)
{
    this->addr = addr;
}

int HashNode::getAddr()
{
    return this->addr;
}

