#pragma once
#include <string>

using namespace std;

namespace Solver {
    // Valid words are candidates[0..n-1]. Next guess must be one of them.
    // Chooses the guess that minimizes expected survivors (uniform secret): min sum of n_f^2 over feedback buckets f.
    string bestGreedyGuess(const string* candidates, int n);
}
