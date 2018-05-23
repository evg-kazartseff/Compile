//
// Created by direnol on 23.05.18.
//

#include "../../inc/HashNode.h"

HashNode::HashNode(int type, std::string &Value)
{
    this->next = this->prev = nullptr;
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

