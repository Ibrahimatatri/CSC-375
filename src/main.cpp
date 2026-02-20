#include <iostream>
#include "Feedback.h"
#include "WordList.h"

using namespace std;

int main() {
    // load all the 5-letter words from our dictionary text file
    WordList wl = loadWordList("data/wordlist.txt");
    cout << "Loaded words: " << wl.count << "\n";

    // variables to hold what the user guessed and what the game told them
    string guess, fb;
    cout << "Enter guess (5 letters): ";
    cin >> guess;
    cout << "Enter feedback (5 chars using G/Y/B): ";
    cin >> fb;

    int kept = 0; // counter for how many words are still valid options
    
    // first pass: just count how many words match the feedback
    for (int i = 0; i < wl.count; i++) {
        // if this dictionary word would give the exact same feedback, it's a valid candidate
        if (Feedback::matches(guess, fb, wl.words[i])) {
            kept++;
        }
    }

    cout << "Candidates remaining: " << kept << "\n";
    cout << "First up to 20 candidates:\n";

    int printed = 0; // track how many we've shown so we don't spam the console
    
    // second pass: loop through the list again to actually print the valid words
    // stop early if we hit 20 printed words
    for (int i = 0; i < wl.count && printed < 20; i++) {
        if (Feedback::matches(guess, fb, wl.words[i])) {
            cout << wl.words[i] << "\n";
            printed++;
        }
    }

    return 0;
}

