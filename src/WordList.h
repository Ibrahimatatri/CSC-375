#pragma once
#include <string>

using namespace std;

struct WordList {
    string* words;
    int count;
    int capacity;

    WordList();
    ~WordList();

    void add(const string& w);
};

WordList loadWordList(const string& path);

