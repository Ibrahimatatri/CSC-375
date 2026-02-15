#pragma once
#include <string>
#include <cstddef>

class HashTable {
public:
    HashTable(std::size_t initialCapacity = 101);   // prime-ish default
    ~HashTable();

    HashTable(const HashTable&) = delete;
    HashTable& operator=(const HashTable&) = delete;

    bool insert(const std::string& key);   // returns true if inserted, false if already existed
    bool contains(const std::string& key) const;
    bool remove(const std::string& key);   // returns true if removed, false if not found

    std::size_t size() const;
    std::size_t capacity() const;
    double loadFactor() const;

    void clear();

private:
    struct Node {
        std::string key;
        Node* next;
        Node(const std::string& k, Node* n) : key(k), next(n) {}
    };

    Node** buckets;              // dynamic array of linked-list heads
    std::size_t bucketCount;
    std::size_t elementCount;

    std::size_t hashKey(const std::string& key) const;
    void rehash(std::size_t newCapacity);

    // helpers
    static bool isPrime(std::size_t n);
    static std::size_t nextPrime(std::size_t n);
};
