#include <iostream>
#include "Feedback.h"
#include "WordList.h"

using namespace std;

int main() {
    WordList wl = loadWordList("data/wordlist.txt");
    cout << "Loaded words: " << wl.count << "\n";

    string guess, fb;
    cout << "Enter guess (5 letters): ";
    cin >> guess;
    cout << "Enter feedback (5 chars using G/Y/B): ";
    cin >> fb;

    int kept = 0;
    for (int i = 0; i < wl.count; i++) {
        if (Feedback::matches(guess, fb, wl.words[i])) {
            kept++;
        }
    }

    cout << "Candidates remaining: " << kept << "\n";
    cout << "First up to 20 candidates:\n";

    int printed = 0;
    for (int i = 0; i < wl.count && printed < 20; i++) {
        if (Feedback::matches(guess, fb, wl.words[i])) {
            cout << wl.words[i] << "\n";
            printed++;
        }
    }

    return 0;
}


