#pragma once
#include <string>

using namespace std;

class DecisionTree {
private:
    struct Node {
        string guess;
        string feedback;
        Node** children;
        int childCount;
        int childCapacity;

        Node(const string& g, const string& f); // create node
        ~Node(); // free children array
    };

    Node* root;

    void destroy(Node* node); // delete tree
    void printPreorder(Node* node, int depth) const; // print helper
    Node* findNode(Node* node, const string& guess);
    void addChild(Node* parent, const string& guess, const string& feedback);

public:
    DecisionTree();
    ~DecisionTree();

    void setRoot(const string& guess, const string& feedback); // set root node
    void addChildToRoot(const string& guess, const string& feedback); // add child
    bool addChildToGuess(const string& parentGuess, const string& guess, const string& feedback);
    bool addRoundNode(const string& parentLabel, const string& roundLabel);
    bool addSuggestionNode(const string& parentLabel, const string& suggestionLabel);
    void printTree() const; // print full tree
    bool isEmpty() const; // check if empty
};

