#include "SuffixTree.h"
#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

string loadFile(const string& filePath) {
    ifstream inFile(filePath);
    
    if (!inFile) {
        cerr << "Error: could not open file " << filePath << endl;
        exit(1);
    }
    string content, line;
    while (getline(inFile, line)) {
        content += line + "\n";
    }
    return content;
}

string toLower(const string& s) {
    string result = s;
    transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

int main() {
    string fileName;
    cout << "Enter file name to index: ";
    cin >> fileName;
    
    string originalText = loadFile(fileName);
    string lowerText = toLower(originalText);
    SuffixTree tree(lowerText, originalText);  // Pass the original text and lowercase version
    
    string query;
    while (true) {
        cout << "\nEnter substring to search (or type '00'): ";
        cin >> query;
        
        if (query == "00") break;
        
        string lowerQuery = toLower(query);  // Lowercase the query for case-insensitive search
        auto positions = tree.search(lowerQuery, true);  // True for case-insensitive search
        
        if (positions.empty()) {
            cout << "No matches found.\n";
        } else {
            cout << "Found at positions: ";
            for (int pos : positions) {
                cout << pos << " ";
            }
            cout << "\n";
            
            // Specify the output file path
            string outputFilePath = "../output.html";
            tree.printWithHighlights(originalText, positions, query.length(), outputFilePath);  // Output using original text
            cout << "Output saved to " << outputFilePath << endl;
        }
    }
    return 0;
}
