#include <iostream>
#include "Feedback.h"

using namespace std;

int main() {
    cout << "Feedback demo:\n";

    string secret = "cigar";
    string guess  = "crane";

    string fb = Feedback::grade(guess, secret);

    cout << "Secret: " << secret << "\n";
    cout << "Guess : " << guess << "\n";
    cout << "FB    : " << fb << "\n";

    cout << "matches(guess, fb, secret): "
         << (Feedback::matches(guess, fb, secret) ? "true" : "false") << "\n";

    cout << "matches(guess, fb, 'crown'): "
         << (Feedback::matches(guess, fb, "crown") ? "true" : "false") << "\n";

    return 0;
}

