# cpp-suffix-tree-search
Term project for CSPB 2270, utilizing a suffix tree to perform word searches.

This project implements a suffix tree to perform efficient substring searches within a large text document.

A suffix tree is a compressed trie of all the suffixes of a given string. It enables fast pattern matching and substring queries. This project uses Ukkonen's algorithm, a linear-time algorithm for building a suffix tree for a given text. Key benefits include the following: search time is proportional to the length of the substring, independent of the document size; it is ideal for repeated search operations on the same text (indexes once for multiple searches). This project supports case-insensitive matching as well as outputs results to output.html in a readable, highlighted format.

Build Instructions
1) git clone <git@github.com:lkreed21/cpp-suffix-tree-search.git>
2) cd cpp-suffix-tree-search
3) g++ -o suffix_tree_search -I../code ../app/main.cpp ../code/SuffixTree.cpp -std=c++11
4) Run executable: ./SuffixTreeSearch

Usage Instructions
1) When prompted, enter the path to the text file to be indexed. You can use either sample text provided or your own.
2) Enter the substring you want to search (case-insensitive).
3) The program will output all matching positions and generate an HTML file (../output.html) with all matches highlighted.
4) The program will continue to prompt for substring searches until the user enters '00' as the query. 

Project Goals
My stretch goal of highlighting the text in an HTML output was completed, although I couldn't get the highlighting to properly mark the query matches. This is my first use of HTML in C++ or at all in a project, so I'm happy I was able to at least get the output to generate properly.
I also wanted to make sure that the query would be case-insensitive in regards to searching the suffix tree/text. Although it took quite a bit of troubleshooting and a bit of a lengthy workaround, the search does now work to get all matches regardless of case.
I didn't plan to implement Ukkonen's algorithm at first, but the project kept overloading, especially with longer text documents, and the time it took to build the tree was something I deemed unacceptable.

This project demonstrates an efficent way to perform multiple substring searches against large text documents such as in text editors or search engines like I mentioned in my proposal. It definitely needs more work if I wanted to use it for work-related purposes, mainly with the highlighting correct positions and the struggle the HTML output has with special characters (including bullet points, which are very prevalent in the documents I work with). 

