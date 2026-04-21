#pragma once
#include <string>

namespace Solver {
    std::string bestGreedyGuess(const std::string* candidates, int n);
    std::string bestBacktrackingGuess(const std::string* candidates, int n);
}
   
