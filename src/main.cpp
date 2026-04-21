#include <iostream>
#include <string>
#include "Feedback.h"
#include "WordList.h"
#include "DecisionTree.h"
#include "Solver.h"

using namespace std;

static bool validFeedback(const string& fb) {
    if (fb.size() != 5) return false;
    for (char c : fb) {
        if (c != 'G' && c != 'Y' && c != 'B') return false;
    }
    return true;
}

int main() {
    WordList wl = loadWordList("data/wordlist.txt");
    cout << "Loaded words: " << wl.count << "\n";

    DecisionTree tree;
    bool hasRoot = false;
    string lastRoundLabel;

    string* currentCandidates = new string[wl.count];
    int currentCount = wl.count;
    for (int i = 0; i < wl.count; i++) {
        currentCandidates[i] = wl.words[i];
    }

    int round = 1;
    while (true) {
        string guess;
        cout << "\nRound " << round << "\n";
        cout << "Enter guess (5 letters) or 'exit': ";
        if (!(cin >> guess)) break;

        if (guess == "exit") {
            cout << "Exiting solver.\n";
            break;
        }

        if (guess.size() != 5) {
            cout << "Guess must be exactly 5 letters.\n";
            continue;
        }

        string fb;
        cout << "Enter feedback (5 chars using G/Y/B): ";
        if (!(cin >> fb)) break;

        if (!validFeedback(fb)) {
            cout << "Feedback must be exactly 5 chars using only G, Y, B.\n";
            continue;
        }

        string roundLabel = "Round " + to_string(round) + ": " + guess + " (Feedback: " + fb + ")";
        if (!hasRoot) {
            tree.setRoot(roundLabel, "");
            hasRoot = true;
        } else {
            if (!tree.addRoundNode(lastRoundLabel, roundLabel)) {
                tree.addChildToRoot(roundLabel, "");
            }
        }
        lastRoundLabel = roundLabel;

        int kept = 0;
        for (int i = 0; i < currentCount; i++) {
            if (Feedback::matches(guess, fb, currentCandidates[i])) {
                kept++;
            }
        }

        string* filtered = nullptr;
        if (kept > 0) {
            filtered = new string[kept];
            int idx = 0;
            for (int i = 0; i < currentCount; i++) {
                if (Feedback::matches(guess, fb, currentCandidates[i])) {
                    filtered[idx++] = currentCandidates[i];
                }
            }
        }

        delete[] currentCandidates;
        currentCandidates = filtered;
        currentCount = kept;

        cout << "Candidates remaining: " << currentCount << "\n";
        cout << "First up to 10 candidates:\n";
        for (int i = 0; i < currentCount && i < 10; i++) {
            cout << currentCandidates[i] << "\n";
        }

        if (fb == "GGGGG") {
            cout << "\nSolved! Nice work.\n";
            break;
        }

        if (currentCount == 0) {
            cout << "\nNo valid candidates remain. Stopping.\n";
            break;
        }

        string greedyGuess = Solver::bestGreedyGuess(currentCandidates, currentCount);
        string backtrackingGuess = Solver::bestBacktrackingGuess(currentCandidates, currentCount);

        cout << "\nGreedy suggested next guess: " << greedyGuess << "\n";
        cout << "Backtracking suggested next guess: " << backtrackingGuess << "\n";

        tree.addSuggestionNode(lastRoundLabel, "Greedy Suggestion: " + greedyGuess);
        tree.addSuggestionNode(lastRoundLabel, "Backtracking Suggestion: " + backtrackingGuess);

        cout << "\nDecision tree so far:\n";
        tree.printTree();

        round++;
    }

    if (hasRoot) {
        cout << "\nFinal decision tree:\n";
        tree.printTree();
    }

    delete[] currentCandidates;

    return 0;
}
