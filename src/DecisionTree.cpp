#include "DecisionTree.h"
#include <iostream>
#include <cctype>

using namespace std;

static bool startsWith(const string& s, const string& prefix) {
    return s.size() >= prefix.size() && s.compare(0, prefix.size(), prefix) == 0;
}

static bool parseRoundLabel(const string& label, int& roundOut, string& guessOut) {
    // Expected stored form: "R<number>: <guess>"
    if (label.size() < 4) return false;
    if (label[0] != 'R') return false;

    int i = 1;
    if (i >= (int)label.size() || !isdigit((unsigned char)label[i])) return false;

    int round = 0;
    while (i < (int)label.size() && isdigit((unsigned char)label[i])) {
        round = round * 10 + (label[i] - '0');
        i++;
    }

    if (i + 2 >= (int)label.size()) return false;
    if (label[i] != ':' || label[i + 1] != ' ') return false;

    roundOut = round;
    guessOut = label.substr(i + 2);
    return true;
}

static string formatNodeLine(const string& storedGuessLabel, const string& storedFeedback) {
    if (startsWith(storedGuessLabel, "Round ")) return storedGuessLabel;
    if (startsWith(storedGuessLabel, "Greedy Suggestion: ")) return storedGuessLabel;
    if (startsWith(storedGuessLabel, "Backtracking Suggestion: ")) return storedGuessLabel;

    // Suggestions are stored like: "SUGG(G): <word>" and "SUGG(B): <word>"
    const string gPrefix = "SUGG(G): ";
    const string bPrefix = "SUGG(B): ";

    if (startsWith(storedGuessLabel, gPrefix)) {
        return "Greedy Suggestion: " + storedGuessLabel.substr(gPrefix.size());
    }
    if (startsWith(storedGuessLabel, bPrefix)) {
        return "Backtracking Suggestion: " + storedGuessLabel.substr(bPrefix.size());
    }

    int round = 0;
    string guess;
    if (parseRoundLabel(storedGuessLabel, round, guess)) {
        return "Round " + to_string(round) + ": " + guess + " (Feedback: " + storedFeedback + ")";
    }

    // Fallback for any other nodes (keeps behavior safe if labels change later).
    return storedGuessLabel + " (Feedback: " + storedFeedback + ")";
}

DecisionTree::Node::Node(const string& g, const string& f) {
    guess = g;
    feedback = f;
    childCount = 0;
    childCapacity = 2;
    children = new Node*[childCapacity];

    for (int i = 0; i < childCapacity; i++) {
        children[i] = nullptr;
    }
}

DecisionTree::Node::~Node() {
    delete[] children; // free array
}

DecisionTree::DecisionTree() {
    root = nullptr;
}

DecisionTree::~DecisionTree() {
    destroy(root); // delete whole tree
}

void DecisionTree::destroy(Node* node) {
    if (node == nullptr) return;

    for (int i = 0; i < node->childCount; i++) {
        destroy(node->children[i]); // delete children
    }

    delete node;
}

bool DecisionTree::isEmpty() const {
    return root == nullptr;
}

void DecisionTree::setRoot(const string& guess, const string& feedback) {
    if (root != nullptr) {
        destroy(root); // reset tree
    }
    root = new Node(guess, feedback);
}

void DecisionTree::addChild(Node* parent, const string& guess, const string& feedback) {
    if (parent == nullptr) return;

    if (parent->childCount >= parent->childCapacity) {
        int newCapacity = parent->childCapacity * 2;
        Node** newChildren = new Node*[newCapacity];

        for (int i = 0; i < parent->childCount; i++) {
            newChildren[i] = parent->children[i];
        }

        for (int i = parent->childCount; i < newCapacity; i++) {
            newChildren[i] = nullptr;
        }

        delete[] parent->children;
        parent->children = newChildren;
        parent->childCapacity = newCapacity;
    }

    parent->children[parent->childCount] = new Node(guess, feedback);
    parent->childCount++;
}

DecisionTree::Node* DecisionTree::findNode(Node* node, const string& guess) {
    if (node == nullptr) return nullptr;
    if (node->guess == guess) return node;

    for (int i = 0; i < node->childCount; i++) {
        Node* found = findNode(node->children[i], guess);
        if (found != nullptr) return found;
    }
    return nullptr;
}

void DecisionTree::addChildToRoot(const string& guess, const string& feedback) {
    addChild(root, guess, feedback);
}

bool DecisionTree::addChildToGuess(const string& parentGuess, const string& guess, const string& feedback) {
    Node* parent = findNode(root, parentGuess);
    if (parent == nullptr) return false;
    addChild(parent, guess, feedback);
    return true;
}

bool DecisionTree::addRoundNode(const string& parentLabel, const string& roundLabel) {
    return addChildToGuess(parentLabel, roundLabel, "");
}

bool DecisionTree::addSuggestionNode(const string& parentLabel, const string& suggestionLabel) {
    return addChildToGuess(parentLabel, suggestionLabel, "");
}

void DecisionTree::printPreorder(Node* node, int depth) const {
    if (node == nullptr) return;

    for (int i = 0; i < depth; i++) {
        cout << "  ";
    }

    cout << formatNodeLine(node->guess, node->feedback) << "\n";

    for (int i = 0; i < node->childCount; i++) {
        printPreorder(node->children[i], depth + 1); // go deeper
    }
}

void DecisionTree::printTree() const {
    if (root == nullptr) {
        cout << "Tree is empty.\n";
        return;
    }

    printPreorder(root, 0);
}

