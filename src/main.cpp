#include <iostream>
#include "HashTable.h"

int main() {
    HashTable ht;

    std::cout << "HashTable demo:\n";
    std::cout << "Insert 'crane': " << (ht.insert("crane") ? "true" : "false") << "\n";
    std::cout << "Insert 'crane' again: " << (ht.insert("crane") ? "true" : "false") << "\n";
    std::cout << "Contains 'crane': " << (ht.contains("crane") ? "true" : "false") << "\n";
    std::cout << "Remove 'crane': " << (ht.remove("crane") ? "true" : "false") << "\n";
    std::cout << "Contains 'crane' after remove: " << (ht.contains("crane") ? "true" : "false") << "\n";

    std::cout << "Size: " << ht.size() << ", Capacity: " << ht.capacity()
              << ", LoadFactor: " << ht.loadFactor() << "\n";

    return 0;
}
