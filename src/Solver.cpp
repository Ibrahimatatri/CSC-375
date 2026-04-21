#include "Solver.h"
#include "Feedback.h"
#include "HashTable.h"

#include <algorithm>
#include <limits>
#include <string>

namespace Solver {

static int countMatchesForPattern(const std::string& guess, const std::string& pattern, const std::string* candidates, int n) {
    int count = 0;
    for (int i = 0; i < n; i++) {
        if (Feedback::grade(guess, candidates[i]) == pattern) count++;
    }
    return count;
}

static int worstPartitionSize(const std::string& guess, const std::string* candidates, int n) {
    HashTable seen;
    std::string patterns[243]; // 3^5 possible feedback strings
    int pCount = 0;

    for (int i = 0; i < n; i++) {
        std::string fb = Feedback::grade(guess, candidates[i]);
        if (seen.insert(fb)) {
            patterns[pCount++] = fb;
        }
    }

    int worst = 0;
    for (int i = 0; i < pCount; i++) {
        int c = countMatchesForPattern(guess, patterns[i], candidates, n);
        worst = std::max(worst, c);
    }
    return worst;
}

// Minimax over feedback buckets: pick guess with smallest worst bucket.
std::string bestGreedyGuess(const std::string* candidates, int n) {
    if (!candidates || n <= 0) return "";
    if (n == 1) return candidates[0];

    int bestWorst = std::numeric_limits<int>::max();
    std::string best = candidates[0];

    for (int gi = 0; gi < n; gi++) {
        const std::string& g = candidates[gi];
        int worst = worstPartitionSize(g, candidates, n);
        if (worst < bestWorst) {
            bestWorst = worst;
            best = g;
        }
    }
    return best;
}

static int backtrackingScore(const std::string* candidates, int n, int depthLeft, HashTable& visited) {
    if (n <= 1) return n;
    if (depthLeft <= 0) {
        std::string g = bestGreedyGuess(candidates, n);
        return worstPartitionSize(g, candidates, n);
    }

    int branchLimit = std::min(n, 18); // simple runtime cap for class project
    int bestScore = std::numeric_limits<int>::max();

    for (int gi = 0; gi < branchLimit; gi++) {
        const std::string& guess = candidates[gi];
        if (visited.contains(guess)) continue;
        visited.insert(guess);

        std::string patterns[243];
        int pCount = 0;
        HashTable seenPattern;
        for (int si = 0; si < n; si++) {
            std::string fb = Feedback::grade(guess, candidates[si]);
            if (seenPattern.insert(fb)) patterns[pCount++] = fb;
        }

        int worstFuture = 0;
        for (int pi = 0; pi < pCount; pi++) {
            const std::string& fb = patterns[pi];
            int subCount = countMatchesForPattern(guess, fb, candidates, n);
            if (subCount <= 1) {
                worstFuture = std::max(worstFuture, subCount);
                continue;
            }

            std::string* subset = new std::string[subCount];
            int idx = 0;
            for (int si = 0; si < n; si++) {
                if (Feedback::grade(guess, candidates[si]) == fb) subset[idx++] = candidates[si];
            }
            int childScore = backtrackingScore(subset, subCount, depthLeft - 1, visited);
            delete[] subset;

            worstFuture = std::max(worstFuture, childScore);
        }

        if (worstFuture < bestScore) bestScore = worstFuture;
        visited.remove(guess);
    }

    if (bestScore == std::numeric_limits<int>::max()) {
        std::string g = bestGreedyGuess(candidates, n);
        return worstPartitionSize(g, candidates, n);
    }
    return bestScore;
}

std::string bestBacktrackingGuess(const std::string* candidates, int n) {
    if (!candidates || n <= 0) return "";
    if (n == 1) return candidates[0];

    int depthLimit = 2;
    int branchLimit = std::min(n, 16);
    int bestScore = std::numeric_limits<int>::max();
    std::string best = candidates[0];
    HashTable visited;

    for (int gi = 0; gi < branchLimit; gi++) {
        const std::string& guess = candidates[gi];
        visited.insert(guess);

        std::string patterns[243];
        int pCount = 0;
        HashTable seenPattern;
        for (int si = 0; si < n; si++) {
            std::string fb = Feedback::grade(guess, candidates[si]);
            if (seenPattern.insert(fb)) patterns[pCount++] = fb;
        }

        int score = 0;
        for (int pi = 0; pi < pCount; pi++) {
            const std::string& fb = patterns[pi];
            int subCount = countMatchesForPattern(guess, fb, candidates, n);
            if (subCount <= 1) {
                score = std::max(score, subCount);
                continue;
            }
            std::string* subset = new std::string[subCount];
            int idx = 0;
            for (int si = 0; si < n; si++) {
                if (Feedback::grade(guess, candidates[si]) == fb) subset[idx++] = candidates[si];
            }
            int childScore = backtrackingScore(subset, subCount, depthLimit - 1, visited);
            delete[] subset;
            score = std::max(score, childScore);
        }

        visited.remove(guess);
        if (score < bestScore) {
            bestScore = score;
            best = guess;
        }
    }
    return best;
}

}  // namespace Solver
