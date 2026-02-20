#include "Feedback.h"

namespace Feedback {

// Evaluates the guess and returns G (green), Y (yellow), or B (black)
string grade(const string& guess, const string& secret) {
    string fb = "BBBBB"; // default to all wrong
    int counts[26] = {0}; // array to track letter frequencies

    // count how many times each letter appears in the secret word
    for (int i = 0; i < 5; i++) {
        counts[secret[i] - 'a']++;
    }

    // pass 1: find exact matches first (Greens)
    for (int i = 0; i < 5; i++) {
        if (guess[i] == secret[i]) {
            fb[i] = 'G';
            counts[guess[i] - 'a']--;
        }
    }

    // pass 2: find wrong position matches (Yellows)
    for (int i = 0; i < 5; i++) {
        if (fb[i] == 'G') continue; // skip letters we already matched
        
        int idx = guess[i] - 'a';
        if (counts[idx] > 0) {
            fb[i] = 'Y';
            counts[idx]--;
        }
    }

    return fb;
}

// checks if a word from the dictionary would give this exact feedback
bool matches(const string& guess, const string& feedback, const string& candidate) {
    return grade(guess, candidate) == feedback;
}

}
