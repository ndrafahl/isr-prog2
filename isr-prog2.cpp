//Author:     Nick Drafahl
//Class:      Information Storage and Retrieval
//Usage:      ./isr-prog2 <file1 [file2] ...>

#include <set>
#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <iterator>
#include <algorithm>

// Custom structure that will store our unique tokens as well as the documents
// that they are found in.
struct token {
    std::string sWord;              // Token
    std::vector <int> docList;      // Vector of all documents token is found in (dupes allowed)
    std::set <int> uniDocList;      // Set of all documents token is found in (dupes not allowed)

    token(const std::string& x) : sWord(x) {}

    void addDocNum(int docNum) {
        docList.push_back(docNum);
        uniDocList.insert(docNum);
    }

    std::string getWord() { return sWord; };
};


void printFileCollection (std::vector <std::string> fCol);
std::string removePunc (std::string word);
bool compTokensLess(const token & t1, const token & t2);
bool compTokensLower(const token & t1, const std::string & t2);
void printTokenVector(std::vector<token> tVector);

int main(int argc, char * argv[]) {

    // Variable Declarations
    std::vector<std::string> fileCollection;
    std::vector<token> tokenVec;

    std::ifstream myFile;

    // Print to CLI if the user didn't pass in any documents to parse
    if (argc <= 1) {
        std::cout << "Usage: This program requires at least one document to be parsed.\n";
        exit(0);
    }

    // Primary loop that loops through all the documents passed in via command
    // line
    for (int i = 1; i < argc; ++i) {
        fileCollection.push_back(argv[i]);
        myFile.open(argv[i]);

        if (!myFile.is_open()) {
            std::cout << "Unable to open file: " << argv[i] << std::endl;
            std::cout << "Usage: " << argv[i] << " document1 [document2] ...\n";
            exit(0);
        }

        std::string word;

        // Loop that pulls all words from a single file
        while (myFile >> word) {
            std::cout << "new input word is: " << word << std::endl;

            // Create iterator to search through the words stored in the
            // tokens stored in our Vector
            //auto b = find_if(tokenVec.begin(), tokenVec.end(), [word](const token& temp) {return temp.sWord == word;});
            auto b = std::lower_bound(tokenVec.begin(), tokenVec.end(), word, compTokensLower);
            auto iVec = std::distance(tokenVec.begin(), b);

            // If we've found a match, we just want to update the document
            // lists for that word.
            /*if(b != tokenVec.end()) {
                auto iVec = std::distance(tokenVec.begin(), b);
                tokenVec.at(iVec).addDocNum(i+1);
            } else { // Otherwise, we need to create a new token and construct it with this document's number
                token temp = token(word);
                temp.addDocNum(i+1);
                tokenVec.push_back(temp);
                std::sort(tokenVec.begin(), tokenVec.end(), compTokensLess);
                //printTokenVector(tokenVec);
            } */

            // We should be able to use lower_bound because we're sorting the
            // Vector anytime we put in a new item.
            if(iVec == tokenVec.size()) {
                //std::cout << "entered if statement\n";
                std::cout << "Inserting token with word " << word << std::endl;
                token temp = token(word);
                temp.addDocNum(i+1);
                tokenVec.push_back(temp);
                //std::cout << "tokenVec size is : " << tokenVec.size() << std::endl;
                std::sort(tokenVec.begin(), tokenVec.end(), compTokensLess);
                std::cout << "Sorted tokenVec\n";
                printTokenVector(tokenVec);
            } else {
                //std::cout << "entered else statement\n";
                int iVec = std::distance(tokenVec.begin(), b);
                tokenVec.at(iVec).addDocNum(i+1);
            }

            //wordCollection.insert(word);
        }

        myFile.close();
    }

    std::cout << "Exited main loop.\n";

    printFileCollection(fileCollection);

    for(int i = 0; i < tokenVec.size(); ++i) {
        std::cout << tokenVec.at(i).sWord << std::endl;
    }

    return 0;
}

// Function takes in a vector containing all of the file names and prints them
// to the console as the Legend
void printFileCollection (std::vector< std::string > fCol) {

    std::cout << "Legend:" << std::endl;
    for (int i = 0; i < fCol.size(); ++i) {
        std::cout << i + 1 << ". " << fCol.at(i) << std::endl;
    }
}

// Function that takes in a word and removes all punctuation
std::string removePunc (std::string word) {
    //bool check = true;

    int iter = 0;

    std::cout << "word is : " << word << " (size: " << word.size() << ")" << std::endl;
    /*for(char c : word) {
        //std::cout << c << std::endl;
        if(c == '-') {
            word.erase(iter);
            //word.resize(word.size() - 1);
        }
        iter++;
    }*/

    for(int i = 0; i < word.size(); ++i) {
        char c = word.at(i);

        if(c == '!') {
            word.erase(i);
            std::cout << "Removing letter at index " << i << std::endl;
            std::cout << "New size is : " << word.size() << std::endl;
        } else if (c == '-') {
            word.erase(i);
        }
    }
    //check = false;

    std::cout << "returning word is : " << word << std::endl;
    return word;
}

// Function used to sort the vector of tokens alphabetically
bool compTokensLess(const token & t1, const token & t2) {
    return t1.sWord < t2.sWord;
}

bool compTokensLower(const token & t1, const std::string & t2) {
    std::cout << "comparing token word " << t1.sWord << "<  input word " << t2;
    if(t1.sWord < t2) { std::cout << " (true)\n";}
    else {std::cout << " (false)\n";}
    return t1.sWord < t2;
}

// Function to print the words stored currently in a vector of tokens
void printTokenVector(std::vector<token> tVector) {
    for(int i = 0; i < tVector.size(); ++i) {
        std::cout << tVector.at(i).sWord << std::endl;
    }
    std::cout << "\n";
}
