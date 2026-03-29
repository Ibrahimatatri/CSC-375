#include <iostream>
#include "Feedback.h"
#include "WordList.h"
#include "DecisionTree.h"
#include "Solver.h"

using namespace std;

int main() {
    WordList wl = loadWordList("data/wordlist.txt");
    cout << "Loaded words: " << wl.count << "\n";

    string guess, fb;
    cout << "Enter guess (5 letters): ";
    cin >> guess;
    cout << "Enter feedback (5 chars using G/Y/B): ";
    cin >> fb;

    DecisionTree tree;
    tree.setRoot(guess, fb);

    int kept = 0;
    for (int i = 0; i < wl.count; i++) {
        if (Feedback::matches(guess, fb, wl.words[i])) {
            kept++;
        }
    }

    cout << "Candidates remaining: " << kept << "\n";
    cout << "First up to 20 candidates:\n";

    string* filtered = new string[kept];
    int index = 0;
    int printed = 0;

    for (int i = 0; i < wl.count; i++) {
        if (Feedback::matches(guess, fb, wl.words[i])) {
            filtered[index] = wl.words[i];

            if (printed < 20) {
                cout << wl.words[i] << "\n";
                printed++;
            }

            index++;
        }
    }

    if (kept > 0) {
        string nextGuess = Solver::bestGreedyGuess(filtered, kept);
        cout << "\nGreedy suggested next guess: " << nextGuess << "\n";

        tree.addChildToRoot(nextGuess, "next");
        cout << "\nDecision tree preview:\n";
        tree.printTree();
    } else {
        cout << "\nNo valid candidates found.\n";
    }

    delete[] filtered;

    return 0;
}
