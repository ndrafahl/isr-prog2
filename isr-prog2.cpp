//Author:     Nick Drafahl
//Class:      Information Storage and Retrieval
//Usage:      ./isr-prog2 <file1 [file2] ...>
//Compile:    g++ -Wall -pedantic -ansi isr-prog2.cpp stemmer.o -o isr-prog2

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
    int uniSize;

    token(const std::string& x) : sWord(x) {}

    void addDocNum(int docNum) {
        docList.push_back(docNum);
        uniDocList.insert(docNum);
    }

    friend bool operator< (const token &left, const token &right);

    std::string getWord() { return sWord; };
    std::set<int> getUniqueDocList() { return uniDocList; }
    int getUniSize() { return uniDocList.size(); }
    void updateUniSize() { uniSize = uniDocList.size(); }
};

// Function Prototypes
void printFileCollection (std::vector <std::string> fCol, int biggestWord);
std::vector<std::string> removePunc (std::string word);
bool compTokensLess(const token & t1, const token & t2);
bool compTokensLower(const token & t1, const std::string & t2);
void printTokenVector(std::vector<token> tVector);
void printIntegerSet(std::set<int> tSet);
extern int stem(char* p, int i, int j);

int main(int argc, char * argv[]) {

    // Variable Declarations
    std::vector<std::string> fileCollection;
    std::vector<token> tokenVec;
    std::set<token> tokenSet;
    unsigned int biggestWord = 0;

    std::ifstream myFile;

    /*std::string test = "Hello";
    int len = test.size();
    char *charArray = &test[0];
    int idx = stem(charArray, 0, len);

    for(int i = 0; i < test.size(); ++i) {
        std::cout << charArray[i];
    } */

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
        std::string newWord;
        std::vector<std::string> puncWords;

        // Loop that pulls all words from a single file
        while (myFile >> word) {
            // Clear puncWords on new word
            puncWords.clear();

            // Call removePunc function on the word, return a vector of words
            // that we will add into the postings list.
            std::cout << "Calling removePunc from main.\n";
            puncWords = removePunc(word);
            std::cout << "Returned to main from removePunc\n";

            // For each words that we've returned from removePunc, verify that
            // the word isn't bigger than the biggest word.  We are going to
            // then see if our set of tokens already contains that word
            for (unsigned int i = 0; i < puncWords.size(); ++i) {
                newWord = puncWords.at(i);

                std::set<token>::iterator c;

                if (newWord.size() > biggestWord) {
                    biggestWord = word.size();
                }

            // Search through our set of tokens.  If we find that the token
            // already exists in the set, let's add this document number to
            // that token's list of document numbers.
                if(tokenSet.find(newWord) != tokenSet.end()) {
                    //std::cout << "Found existing token for: " << newWord << std::endl;
                    token temp = *c;
                    temp.addDocNum(i+1);
                    temp.updateUniSize();
                    tokenSet.erase(c);
                    tokenSet.insert(temp);
                    //std::cout << "Token's unique size is: " << temp.getUniSize() << std::endl;
                    //printIntegerSet(temp.uniDocList);

                // Otherwise, create a new token because we've never seen this
                // word before
                } else {
                    token temp = token(newWord);
                    temp.addDocNum(i+1);
                    tokenSet.insert(temp);
                } //end if..else
            } //end (unsigned int... (process words, add to token set)
        } //end while(myFile... (pull words from file)

        myFile.close();
    } //end for(int i... (read through documents)

    std::cout << "Exited main loop.\n";

    printFileCollection(fileCollection, biggestWord); // Print Legend

    // For each token in the set of tokens, loop through and print the word
    // and the postings list for that word
    std::set<token>::iterator iter;
    for(iter = tokenSet.begin(); iter != tokenSet.end(); ++iter) {
        token t = *iter;
        std::cout << t.sWord;

        for (unsigned int i = t.sWord.size() -1; i < biggestWord; ++i) {
            std::cout << " ";
        }

        printIntegerSet(t.uniDocList); // Print document numbers for that token
    }

    return 0;
}

// Function takes in a vector containing all of the file names and prints them
// to the console as the Legend
void printFileCollection (std::vector< std::string > fCol, int biggestWord) {

    std::cout << "Legend:" << std::endl;
    for(unsigned int i = 0; i < fCol.size(); ++i) {
        std::cout << i + 1 << ". " << fCol.at(i) << std::endl;
    }

    // Everything that follows is some magical way of formatting the Legend
    std::cout << "" << std::endl;

    std::cout << "Word";
    for(int i = 4; i < biggestWord; ++i) {
        std::cout << " ";
    }

    std::cout << " Posting\n";

    for(int i = 0; i < biggestWord; ++i) {
        std::cout << "-";
    }

    std::cout << " ";

    for(int i = 0; i < biggestWord; ++i) {
        std::cout << "-";
    }

    std::cout << "" << std::endl;
}

// Function that takes in a word and removes all punctuation
std::vector<std::string> removePunc (std::string word) {
    std::cout << "Entering removePunc\n";

    std::string retString;
    std::vector<std::string> retSVec;

    std::cout << "word is : " << word << " (size: " << word.size() << ")" << std::endl;
    //for(char c : word) {
    for(unsigned int i = 0; i < word.size(); ++i) {
        //std::cout << "c is: " << c << std::endl;

        // If the letter is a puncuation
        /*if(c == '!' || c == '?' || c == '-' || c == ',' || c == '\"' || c == ')' || c == '(' || c == ':'
                || c == '.' || c == '&' || c == '%' || c == '[' || c == ']' || c == ';' ) {*/
        if(word[i] == '!' || word[i] == '?' || word[i] == '-' || word[i] == ',' || word [i] == '\"' || word[i] == ')'
                || word[i] == '(' || word[i] == ':' || word[i] == '.' || word [i] == '&' || word[i] == '%'
                || word[i] == '[' || word[i] == ']' || word[i] == ';' ) {
            // If the letter following the puctuation isn't white space (i.e
            // not the end of the word, maybe it's two words put together
            //if (c+1 != ' ') {
            if(word[i+1] != ' ') {
                // If not working with a blank string at this point (this
                // would only happen if we had two punctations in a row,
                // (i.e something like test!!test2), let's save our previous
                // word into the return vector and clear it to make way for
                // the next word
                if (retString.size() != 0) {
                    std::cout << "Pushing " << retString << " back\n";
                    retSVec.push_back(retString);
                    retString.clear();
                } else {
                    // Do nothing, because retString had no data
                }
            // Else, the following spot was white space after punctuation, nothing more to do
            } else {
                //std::cout << "Did not push something back\n";
            }
        /*} else if (c == '\'') {
            std::cout << "Entering \' land.\n";
            if ( c-1 == 'n' && c+1 == 't') {
                std::cout << "Entering n\'t land.\n";
                retString.erase(retString.end(), retString.end());
                std::cout << "retString after removal: " << retString << std::endl;
                retSVec.push_back(retString);
                retSVec.push_back("not");
                return "test";
            } */
        } else {
            //retString.push_back(c);
            retString.push_back(word[i]);
        } // end else
    } //end for

    // If we have some word leftover still in retString, that means there was
    // still one word that needs to be returned to main to be added into the
    // document collection
    if (retString.size() > 0) {
        std::cout << "Outside of for looping, pushing back " << retString << std::endl;
        retSVec.push_back(retString);
        std::cout << "Successfully pushed back: " << retString << std::endl;
    }
    std::cout << "Passed through retString.size() != 0\n";

    std::cout << "Returning from removePunc with vector size of " << retSVec.size() << "\n\n";

    return retSVec;
} // end removePunc

// Function to print the words stored currently in a vector of tokens
void printTokenVector(std::vector<token> tVector) {
    for(unsigned int i = 0; i < tVector.size(); ++i) {
        std::cout << tVector.at(i).sWord << std::endl;
    }
    std::cout << "\n";
}

// Function to take in a set of Integers (in this case, the list of documents
// a token was found in) and then print them to the screen.
void printIntegerSet(std::set<int> tSet) {
    std::set<int>::iterator iter;

    //std::cout << "size of (" << tSet.size() << ") ";
    for(iter = tSet.begin(); iter != tSet.end(); ++iter) {
        std::cout << *iter << " ";
    }

    std::cout << "" << std::endl;
}

// overloading the < operator that allows putting our tokens into a set
bool operator< (const token &left, const token &right) {
    return left.sWord < right.sWord;
}

