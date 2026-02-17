// Copyright Andrew Marin 02/17/2026

#include <iostream>
#include <fstream>

int countWords(std::fstream& fs) {
    std::string word;
    int count = 0;

    while (fs >> word) {
        count++;
    }

    return count;
}

int main(int argc, char* argv[]) {
    std::string fileName = "";
    std::fstream fs(fileName);
    while (!fs) {
        std::cout << "Which file do you want to read: ";
        std::cin >> fileName;
        fs.open(fileName);
        if (fs) {
            std::cout << "\nThere are " << countWords(fs)
            << " words in the file." << std::endl;
            return 0;
        } else {
            std::cerr << "Cannot open file. Try again." << std::endl;
        }
    }

    return 0;
}
