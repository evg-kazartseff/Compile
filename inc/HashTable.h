//
// Created by evgenii on 06.03.18.
//

#ifndef LAB2_HASHTABLE_H
#define LAB2_HASHTABLE_H

#include <cstddef>
#include <string>

class HashNode {
private:
    std::string value;
    HashNode* next;
    HashNode* prev;
public:
    explicit HashNode(std::string& Value);
    ~HashNode();
    void setNext(HashNode* next);
    void setPrev(HashNode* prev);
    HashNode* getNext() const;
    HashNode* getPrev() const;
    std::string& getValue();
    void setValue(std::string& value);
};

class HashEntry {
private:
    HashNode* list;
    void DeleteNode(HashNode* node);
public:
    HashEntry() = default;
    ~HashEntry();
    void AddValueNode(std::string value);
    HashNode* LookupValueNode(const std::string& value);
    void DeleteValueNode(const std::string& value);
};

class HashTable {
private:
    size_t size;
    HashEntry* table;
    size_t GetHash(char* key);
public:
    explicit HashTable(size_t size);
    ~HashTable();
    void CreateEntry(char* value);
    HashNode* LookupEntry(char* value);
    void DeleteEntry(char* value);
};

#endif //LAB2_HASHTABLE_H
