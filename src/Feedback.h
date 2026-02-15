#pragma once
#include <string>

using namespace std;

namespace Feedback {
    string grade(const string& guess, const string& secret);
    bool matches(const string& guess, const string& feedback, const string& candidate);
}

