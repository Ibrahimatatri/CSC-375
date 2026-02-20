#include "WordList.h"
#include <fstream>

WordList::WordList() {
    // start with a decent chunk of memory so we don't have to resize right away
    capacity = 1024;
    count = 0;
    words = new string[capacity];
}

WordList::~WordList() {
    // gotta delete the array we created with 'new' to avoid memory leaks
    delete[] words;
    words = nullptr;
    count = 0;
    capacity = 0;
}

void WordList::add(const string& w) {
    // if we ran out of space, double the size of the array
    if (count >= capacity) {
        int newCap = capacity * 2;
        string* newArr = new string[newCap]; // make a bigger array
        
        // copy all the old words into the new array
        for (int i = 0; i < count; i++) newArr[i] = words[i];
        
        // delete the old small array and point to the new big one
        delete[] words;
        words = newArr;
        capacity = newCap;
    }
    
    // add the new word to the end and then increment the count
    words[count++] = w;
}

// helper function to read our dictionary text file
WordList loadWordList(const string& path) {
    WordList wl;
    ifstream in(path); // open the file
    string w;

    // read word by word until the end of the file
    while (in >> w) {
        // only keep the word if it's exactly 5 letters long (since this is Wordle)
        if ((int)w.size() == 5) wl.add(w);
    }
    return wl;
}
