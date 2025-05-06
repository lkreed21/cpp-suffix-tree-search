#ifndef SUFFIXTREE_H
#define SUFFIXTREE_H

#include <string>
#include <vector>
#include <unordered_map>
using namespace std;

// Basic suffix tree implementation for substring search
class SuffixTree {
public:
    struct Node {
        unordered_map<char, Node*> children;
        int start;
        int* end;
        Node* suffixLink;
        vector<int> indices;
        
        Node(int start = -1, int* end = nullptr) : start(start), end(end), suffixLink(nullptr) {}
    };
    
    SuffixTree(const string& inputText, const string& originalText);
    ~SuffixTree();
    
    // Searches for substring in original text
    // Returns the starting position of all matches
    vector<int> search(const string& query, bool caseInsensitive = false);
    void printWithHighlights(const string& text, const vector<int>& positions, int matchLength, const string& outputFilePath);
    
private:
    Node* root;
    Node* activeNode;
    string text;
    string originalText;
    int activeEdge;
    int activeLength;
    int remainingSuffixCount;
    int* globalEnd;
    int size;
    
    void buildTree();
    void extendSuffixTree(int pos);
    bool walkDown(Node* nextNode);
    void setSuffixLink(Node* node);
    void collectIndices(Node* node, vector<int>& result);
    void deleteNodes(Node* node);
};

#endif