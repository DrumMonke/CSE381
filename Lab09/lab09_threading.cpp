// Copyright (c) Andrew Marin 2026
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <iomanip>
#include <thread>

using std::string, std::vector, std::map, std::thread;
using std::cout, std::cerr, std::endl, std::setw, std::left;

// Count the number of times findWord appears in the vector text
// Use count to return the value to the caller.  Note that this is
// an abstracted function since in the main program it will use
// elements of a map to provide findWord and count.
void countWord(const vector<string>& text, const string& findWord, int& count) {
    // Initialize count here in case it wasn't in the main program.
    count = 0;

    // Loop through each element of text
    for (auto& word : text) {
        // If word and findWord are identical, compare will return 0
        if (word.compare(findWord) == 0) {
            // The strings are the same, so increment the counter
            count++;
        }
    }
}

// Open a file with text and read every individual word in the text into
// a std::vector<std::string>
void readText(vector<string>& text, const string& fileName) {
    // Create an ifstream based on the fileName provided
    std::ifstream textFile(fileName);

    // If the file is open, we read from it
    if (textFile.is_open()) {
        // Read until we reach the end of the file
        string temp;
        while (textFile >> temp) {
            // Add the word to the end of the vector
            text.push_back(temp);
        }
        // Close the file!
        textFile.close();
    // If we couldn't read the file, give an error message and exit
    } else {
        cerr << "File not found" << endl;
    }
}

int main(int argc, char* argv[]) {
    // Make sure that a single command-line argument has been provided.  Give
    // an error message and exit if not
    // ADD CODE HERE!
    if (argc != 2) {
        cerr << "Usage: lab09 inputFile" << endl;
        return 0;
    }
    // Now, try to open the file and read its contents.  If the file can't be
    // read, readText will exit with an error message
    // Create a vector to hold the text
    // ADD CODE HERE
    std::string inputFile = argv[1];
    std::vector<std::string> text;
    readText(text, inputFile);
    if (text.empty()) {
        return 0;
    }

    // If we've made it here, we have enough to continue.  We want to count
    // how many times specific words show up.  A std::map is a good way to
    // do that.  Here is a map of the 6 words we're looking for, all
    // initialized to zero
    map<string, int> common = { {"the", 0}, {"as", 0}, {"while", 0},
                                {"The", 0}, {"A", 0},  {"across", 0}};


    // Loop through the common words and for each one count the number of times
    // each word in the map shows up.  You can use the countWord function that
    // is provided.
    std::vector<std::thread> threads;

    for (auto& entry : common) {
        // ADD CODE HERE.  THIS WOULD BE A GOOD PLACE TO CREATE YOUR THREADS
        // IF YOU ARE DOING THE MULTITHREADED VERSION
        threads.push_back(std::thread(countWord, std::ref(text),
        std::ref(entry.first), std::ref(entry.second)));
    }

    for (auto& t : threads) {
        t.join();
    }

    // Print out the results:  Give the word and the count of times it
    // was found in the text.  You should left justify the output and the first
    // field should have a width of 10
    for (auto& entry : common) {
        // ADD CODE HERE
        cout << left << setw(11) << entry.first << entry.second << endl;
    }

    // Exiting here!
    return 0;
}
