#include "WordList.h"
#include <fstream>

WordList::WordList() {
    capacity = 1024;
    count = 0;
    words = new string[capacity];
}

WordList::~WordList() {
    delete[] words;
    words = nullptr;
    count = 0;
    capacity = 0;
}

void WordList::add(const string& w) {
    if (count >= capacity) {
        int newCap = capacity * 2;
        string* newArr = new string[newCap];
        for (int i = 0; i < count; i++) newArr[i] = words[i];
        delete[] words;
        words = newArr;
        capacity = newCap;
    }
    words[count++] = w;
}

WordList loadWordList(const string& path) {
    WordList wl;
    ifstream in(path);
    string w;

    while (in >> w) {
        if ((int)w.size() == 5) wl.add(w);
    }
    return wl;
}

