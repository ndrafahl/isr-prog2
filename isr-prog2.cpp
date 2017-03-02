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

void printFileCollection (std::vector <std::string> fCol);
std::string removePunc (std::string word);

int main(int argc, char * argv[]) {

    std::vector< std::set<std::string> > docCollection;
    std::set<std::string> wordCollection;
    std::set<std::string> dictionary;
    std::vector< std::string > fileCollection;

    std::ifstream myFile;

    if (argc <= 1) {
        std::cout << "Usage: This program requires at least one document to be parsed.\n";
        exit(0);
    }

    for (int i = 1; i < argc; ++i) {
        fileCollection.push_back(argv[i]);
        myFile.open(argv[i]);

        if (!myFile.is_open()) {
            std::cout << "Unable to open file: " << argv[i] << std::endl;
            std::cout << "Usage: " << argv[i] << " document1 [document2] ...\n";
            exit(0);
        }

        std::string word;

        // Read each word from the opened file and place it into a set.
        // This is to prevent duplicate words from being stored on initial
        // read.
        while (myFile >> word) {
            word = removePunc(word);
            wordCollection.insert(word);
        }

        // Push_back the created set into a vector to contain every document
        // we open.
        // Then clear the temporary set for next iteration.
        docCollection.push_back(wordCollection);
        wordCollection.clear();

        myFile.close();
    }

    // For each set we created from the documents, loop through them and
    // insert them into a single set that will be our dictionary.
    // This auto-sorts the dictionary alphabetically and removes duplicates.
    for (int i = 0; i < docCollection.size(); ++i) {
        wordCollection = docCollection.at(i);

        for (std::set<std::string>::iterator iter = wordCollection.begin();
             iter != wordCollection.end(); ++iter) {
            std::string word = *iter;
            dictionary.insert(word);
        }
    }

    // For each item in the dictionary, loop through it and output it to the
    // console
    for (std::set<std::string>::iterator iter = dictionary.begin();
         iter != dictionary.end(); ++iter) {
            std::string word = *iter;
            std::cout << word << std::endl;
    }

    printFileCollection(fileCollection);

    std::string testword = removePunc ("Hello");

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

