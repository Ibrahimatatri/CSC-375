#include "Feedback.h"

namespace Feedback {

string grade(const string& guess, const string& secret) {
    string fb = "BBBBB";
    int counts[26] = {0};

    for (int i = 0; i < 5; i++) {
        counts[secret[i] - 'a']++;
    }

    for (int i = 0; i < 5; i++) {
        if (guess[i] == secret[i]) {
            fb[i] = 'G';
            counts[guess[i] - 'a']--;
        }
    }

    for (int i = 0; i < 5; i++) {
        if (fb[i] == 'G') continue;
        int idx = guess[i] - 'a';
        if (counts[idx] > 0) {
            fb[i] = 'Y';
            counts[idx]--;
        }
    }

    return fb;
}

bool matches(const string& guess, const string& feedback, const string& candidate) {
    return grade(guess, candidate) == feedback;
}

}

