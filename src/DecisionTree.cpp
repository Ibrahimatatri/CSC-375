#include "DecisionTree.h"
#include <iostream>

using namespace std;

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

void DecisionTree::addChildToRoot(const string& guess, const string& feedback) {
    if (root == nullptr) return;

    // resize if needed
    if (root->childCount >= root->childCapacity) {
        int newCapacity = root->childCapacity * 2;
        Node** newChildren = new Node*[newCapacity];

        for (int i = 0; i < root->childCount; i++) {
            newChildren[i] = root->children[i];
        }

        for (int i = root->childCount; i < newCapacity; i++) {
            newChildren[i] = nullptr;
        }

        delete[] root->children;
        root->children = newChildren;
        root->childCapacity = newCapacity;
    }

    root->children[root->childCount] = new Node(guess, feedback);
    root->childCount++;
}

void DecisionTree::printPreorder(Node* node, int depth) const {
    if (node == nullptr) return;

    for (int i = 0; i < depth; i++) {
        cout << "  ";
    }

    cout << "Guess: " << node->guess << ", Feedback: " << node->feedback << "\n";

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

