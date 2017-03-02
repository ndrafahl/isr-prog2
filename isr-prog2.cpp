#include <set>
#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>

int main(int argc, char * argv[]) {

    std::vector< std::set<std::string> > docCollection;
    std::set<std::string> wordCollection;
    std::set<std::string> dictionary;

    std::ifstream myFile;

    if (argc <= 1) {
        std::cout << "Usage: This program requires at least one document to be parsed.\n";
        exit(0);
    }

    for (int i = 1; i < argc; ++i) {
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

    return 0;

}
