#include "HashTable.h"
#include <stdexcept>

HashTable::HashTable(std::size_t initialCapacity)
    : buckets(nullptr), bucketCount(0), elementCount(0)
{
    // make sure the starting size is at least 5 and is a prime number for better distribution
    bucketCount = nextPrime(initialCapacity < 5 ? 5 : initialCapacity);
    buckets = new Node*[bucketCount];
    
    // initialize all buckets to null so we don't have garbage pointers
    for (std::size_t i = 0; i < bucketCount; i++) buckets[i] = nullptr;
}

HashTable::~HashTable() {
    // clean up memory when the object is destroyed so we don't leak
    clear();
    delete[] buckets;
    buckets = nullptr;
    bucketCount = 0;
}

bool HashTable::contains(const std::string& key) const {
    std::size_t idx = hashKey(key);
    Node* cur = buckets[idx];
    
    // walk down the linked list at this bucket to see if our key is there
    while (cur) {
        if (cur->key == key) return true;
        cur = cur->next;
    }
    return false;
}

bool HashTable::insert(const std::string& key) {
    if (contains(key)) return false; // don't allow duplicate keys

    // Rehash threshold (separate chaining can handle >1, but keep reasonable)
    // if it's getting too full, double the size to avoid long linked lists
    if (loadFactor() > 0.75) {
        rehash(nextPrime(bucketCount * 2));
    }

    std::size_t idx = hashKey(key);
    // put the new node at the front of the list (it's faster than traversing to the end)
    buckets[idx] = new Node(key, buckets[idx]); // push front
    elementCount++;
    return true;
}

bool HashTable::remove(const std::string& key) {
    std::size_t idx = hashKey(key);
    Node* cur = buckets[idx];
    Node* prev = nullptr; // need to keep track of the previous node to fix pointers

    while (cur) {
        if (cur->key == key) {
            // if we are not at the head, link the previous node to the next one
            if (prev) prev->next = cur->next;
            // if we are at the head, just update the bucket pointer itself
            else buckets[idx] = cur->next;

            delete cur;
            elementCount--;
            return true;
        }
        prev = cur;
        cur = cur->next;
    }
    return false; // key wasn't in the table
}

std::size_t HashTable::size() const {
    return elementCount;
}

std::size_t HashTable::capacity() const {
    return bucketCount;
}

double HashTable::loadFactor() const {
    if (bucketCount == 0) return 0.0;
    // calculate how full the table is on average
    return static_cast<double>(elementCount) / static_cast<double>(bucketCount);
}

void HashTable::clear() {
    if (!buckets) return;
    
    // go through every single bucket and delete all nodes in their linked lists
    for (std::size_t i = 0; i < bucketCount; i++) {
        Node* cur = buckets[i];
        while (cur) {
            Node* nxt = cur->next; // save next before deleting current
            delete cur;
            cur = nxt;
        }
        buckets[i] = nullptr;
    }
    elementCount = 0;
}

std::size_t HashTable::hashKey(const std::string& key) const {
    // djb2 hash (simple and solid for strings)
    unsigned long long h = 5381ULL;
    for (char c : key) {
        h = ((h << 5) + h) + static_cast<unsigned long long>(static_cast<unsigned char>(c));
    }
    // mod by bucketCount so the index actually fits in our array
    return static_cast<std::size_t>(h % bucketCount);
}

void HashTable::rehash(std::size_t newCapacity) {
    newCapacity = nextPrime(newCapacity);
    // make a new, bigger array of buckets
    Node** newBuckets = new Node*[newCapacity];
    for (std::size_t i = 0; i < newCapacity; i++) newBuckets[i] = nullptr;

    // Move nodes
    for (std::size_t i = 0; i < bucketCount; i++) {
        Node* cur = buckets[i];
        while (cur) {
            Node* nxt = cur->next; // save next before changing cur->next

            // re-hash into new table (have to recalculate because the capacity changed)
            unsigned long long h = 5381ULL;
            for (char c : cur->key) {
                h = ((h << 5) + h) + static_cast<unsigned long long>(static_cast<unsigned char>(c));
            }
            std::size_t newIdx = static_cast<std::size_t>(h % newCapacity);

            // push front into the new bucket
            cur->next = newBuckets[newIdx];
            newBuckets[newIdx] = cur;

            cur = nxt;
        }
        buckets[i] = nullptr;
    }

    // get rid of the old smaller array
    delete[] buckets;
    buckets = newBuckets;
    bucketCount = newCapacity;
    // elementCount unchanged
}

// helper math functions to keep bucket sizes prime
bool HashTable::isPrime(std::size_t n) {
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (std::size_t i = 3; i * i <= n; i += 2) {
        if (n % i == 0) return false;
    }
    return true;
}

std::size_t HashTable::nextPrime(std::size_t n) {
    while (!isPrime(n)) n++;
    return n;
}
