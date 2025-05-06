#include "SuffixTree.h"
#include <cctype>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <locale>
#include <codecvt>
using namespace std;

SuffixTree::SuffixTree(const string& inputText, const string& original) : text(inputText), originalText(original), size(inputText.size()) {
    root = new Node();
    globalEnd = new int(-1);
    activeNode = root;
    activeEdge = -1;
    activeLength = 0;
    remainingSuffixCount = 0;
    
    buildTree();
}

void SuffixTree::buildTree() {
    for (int i = 0; i < size; ++i) {
        extendSuffixTree(i);
    }
}

void SuffixTree::extendSuffixTree(int pos) {
    (*globalEnd)++;
    remainingSuffixCount++;
    Node* lastNewNode = nullptr;

    while (remainingSuffixCount > 0) {
        if (activeLength == 0) {
            activeEdge = pos;
        }

        char currentChar = text[activeEdge];

        if (activeNode->children.find(currentChar) == activeNode->children.end()) {
            Node* leaf = new Node(pos, globalEnd);  // Store positions in the tree
            leaf->indices.push_back(pos);  // Indices are positions in the lowercase text
            activeNode->children[currentChar] = leaf;

            if (lastNewNode) {
                lastNewNode->suffixLink = activeNode;
                lastNewNode = nullptr;
            }
        } else {
            Node* next = activeNode->children[currentChar];
            int edgeLength = *(next->end) - next->start + 1;
            if (activeLength >= edgeLength) {
                activeEdge += edgeLength;
                activeLength -= edgeLength;
                activeNode = next;
                continue;
            }

            if (text[next->start + activeLength] == text[pos]) {
                activeLength++;
                if (lastNewNode) {
                    lastNewNode->suffixLink = activeNode;
                }
                break;
            }

            int* splitEnd = new int(next->start + activeLength - 1);
            Node* split = new Node(next->start, splitEnd);
            activeNode->children[currentChar] = split;

            Node* leaf = new Node(pos, globalEnd);
            leaf->indices.push_back(pos);
            split->children[text[pos]] = leaf;

            next->start += activeLength;
            split->children[text[next->start]] = next;

            if (lastNewNode) {
                lastNewNode->suffixLink = split;
            }

            lastNewNode = split;
        }

        remainingSuffixCount--;
        if (activeNode == root && activeLength > 0) {
            activeLength--;
            activeEdge = pos - remainingSuffixCount + 1;
        } else if (activeNode != root) {
            activeNode = activeNode->suffixLink ? activeNode->suffixLink : root;
        }
    }
}

bool caseInsensitiveCompare(char a, char b) {
    return tolower(a) == tolower(b);
}

bool SuffixTree::walkDown(Node* nextNode) {
    int edgeLength = *(nextNode->end) - nextNode->start + 1;
    if (activeLength >= edgeLength) {
        activeEdge += edgeLength;
        activeLength -= edgeLength;
        activeNode = nextNode;
        return true;
    }
    return false;
}

void SuffixTree::setSuffixLink(Node* node) {
    if (node != root) node-> suffixLink = root;
}

void SuffixTree::collectIndices(Node* node, vector<int>& result) {
    if (!node) return;
    if (!node->children.empty()) {
        for (auto& pair : node->children) {
            collectIndices(pair.second, result);
        }
    } else {
        result.insert(result.end(), node->indices.begin(), node->indices.end());
    }
}

vector<int> SuffixTree::search(const string& query, bool caseInsensitive) {
    Node* current = root;
    int idx = 0;
    
    vector<int> result;
    
    // Perform the search in the suffix tree, as before, but ensure we are mapping the indices correctly
    while (idx < query.size()) {
        char qChar = tolower(query[idx]);

        if (current->children.find(qChar) == current->children.end()) {
            return {};
        }
        Node* next = current->children[qChar];
        int edgeLen = *(next->end) - next->start + 1;

        for (int i = 0; i < edgeLen && idx < query.size(); ++i, ++idx) {
            if (tolower(text[next->start + i]) != tolower(query[idx])) {
                return {};
            }
        }
        current = next;
    }

    // Collecting the positions and mapping them to the original text
    collectIndices(current, result);  // Collect indices from the tree

    // Debugging output for matched positions in the original text
    vector<int> originalIndices;
    for (int pos : result) {
        // Directly use the position from the result because the positions in 'result' are already correct.
        originalIndices.push_back(pos);
    }

    sort(originalIndices.begin(), originalIndices.end());
    return originalIndices;
}

string escapeHTML(const string& input) {
    string result;
    for (char c : input) {
        switch (c) {
            case '<': result += "&lt;"; break;
            case '>': result += "&gt;"; break;
            case '&': result += "&amp;"; break;
            case '"': result += "&quot;"; break;
            case '\'': result += "&apos;"; break;
            case ',': result += "&comma;"; break;
            case '.': result += "&period;"; break;
            default: result += c; break;
        }
    }
    return result;
}

void SuffixTree::printWithHighlights(const string& text, const vector<int>& positions, int matchLength, const string& outputFilePath) {
    ofstream outFile(outputFilePath);
    if (!outFile.is_open()) {
        cerr << "Error opening file for output!" << endl;
        return;
    }

    outFile << "<html><body>\n";
    outFile << "<h1>Search Results</h1>\n<p>";

    size_t lastPos = 0;

    for (size_t i = 0; i < positions.size(); ++i) {
        size_t pos = positions[i];

        // Output text before match
        if (pos > lastPos) {
            outFile << escapeHTML(text.substr(lastPos, pos - lastPos));
        }

        // Output highlighted match
        outFile << "<mark>" << escapeHTML(text.substr(pos, matchLength)) << "</mark>";

        lastPos = pos + matchLength;
    }

    // Output any remaining text after the last match
    if (lastPos < text.length()) {
        outFile << escapeHTML(text.substr(lastPos));
    }

    outFile << "</p>\n</body></html>\n";
    outFile.close();
}

SuffixTree::~SuffixTree() {
    deleteNodes(root);
    delete globalEnd;
}

void SuffixTree::deleteNodes(Node* node) {
    for (auto& pair : node->children) {
        deleteNodes(pair.second);
    }
    if (node->end != globalEnd) {
        delete node->end;
    }
    delete node;
}
