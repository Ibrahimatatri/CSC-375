#include "Solver.h"
#include "Feedback.h"
#include <unordered_map>

namespace Solver {

    // Pick a guess that spreads answers across feedback buckets (smaller sum of squares = better).
    // candidates: possible secret words; n: how many entries are valid.
    string bestGreedyGuess(const string* candidates, int n) {
        if (n <= 0) return "";
        if (n == 1) return candidates[0];

        string best = candidates[0];       // best guess found so far
        long long bestSumSq = -1;          // its score (-1 means "no score yet")

        // Try each word as the guess.
        for (int g = 0; g < n; g++) {      // g: which candidate we are trying as the guess
            const string& guessWord = candidates[g];
            unordered_map<string, int> bucket; // feedback string -> how many answers produce it
            bucket.reserve(128);

            // Count how many answers give each feedback string.
            for (int i = 0; i < n; i++) {  // i: index of a possible answer
                string fb = Feedback::grade(guessWord, candidates[i]); // feedback if secret were candidates[i]
                bucket[fb]++;
            }

            // Sum of squared bucket sizes (same feedback = same bucket).
            long long sumSq = 0;             // score for this guessWord
            for (const auto& p : bucket) {   // p: one (feedback, count) pair
                long long c = p.second;      // count in that bucket
                sumSq += c * c;
            }

            // Best = lowest sumSq; ties go to the smaller word alphabetically.
            if (sumSq < bestSumSq || bestSumSq < 0 || (sumSq == bestSumSq && guessWord < best)) {
                bestSumSq = sumSq;
                best = guessWord;
            }
        }

        return best;
    }

} // namespace Solver
