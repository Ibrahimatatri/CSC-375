#pragma once
#include <string>

namespace Solver {
    std::string bestGreedyGuess(const std::string* candidates, int n);
}
    // Valid words are candidates[0..n-1]. Next guess must be one of them.
    // Chooses the guess that minimizes expected survivors (uniform secret): min sum of n_f^2 over feedback buckets f.
   
