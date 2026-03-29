#include "Solver.h"
#include "Feedback.h"

#include <algorithm>
#include <limits>
#include <string>
#include <unordered_map>

namespace Solver {

// Minimax over feedback buckets: pick the guess whose largest partition
// (when grading against every remaining candidate) is smallest.
std::string bestGreedyGuess(const std::string* candidates, int n) {
    if (!candidates || n <= 0) {
        return "";
    }
    if (n == 1) {
        return candidates[0];
    }

    int bestWorst = std::numeric_limits<int>::max();
    std::string best = candidates[0];

    for (int gi = 0; gi < n; gi++) {
        const std::string& g = candidates[gi];
        std::unordered_map<std::string, int> bucket;
        for (int si = 0; si < n; si++) {
            std::string fb = Feedback::grade(g, candidates[si]);
            bucket[fb]++;
        }
        int worst = 0;
        for (const auto& p : bucket) {
            worst = std::max(worst, p.second);
        }
        if (worst < bestWorst) {
            bestWorst = worst;
            best = g;
        }
    }
    return best;
}

}  // namespace Solver
