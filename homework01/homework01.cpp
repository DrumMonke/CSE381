// Copyright Anrew Marin (c) 2026

#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <cstring>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>
#include <iomanip>

using filePair = std::pair<int, std::string>;
using IntStrPairVec = std::vector<filePair>;
using TypeCount = std::map<std::string, int>;
#define NPOS std::string::npos


// This function loads both the vector of pair objects with
// the file name and size. It also loads the map with the count of file types.
void loadVectorAndMap(std::string dirPath, DIR* directory,
                        IntStrPairVec* fileVector, TypeCount* map) {
    struct dirent *dir;
    struct stat info;
    std::string temp;
    char filePath[1024];

    // We need to use readdir in a loop in order to acces all
    // the files inside the provide directory stream
    while ((dir = readdir(directory)) != NULL) {
        if (dir->d_name[0] != '.') {
            // Inside the loop I gather the files information using stat
            // and store it in the vector along with its name
            std::string path = dirPath + "/%s";
            snprintf(filePath, sizeof(filePath), &path[0], dir->d_name);
            stat(filePath, &info);
            if (S_ISREG(info.st_mode)) {
                fileVector->push_back(std::pair(info.st_size, dir->d_name));
                // To load the map I decided to make a stringstream and use
                // getline using a delimiter to find the file extension
                // and use the extension to increment the count in map
                std::string name = dir->d_name;
                size_t i = name.find(".");
                std::string extension = i != NPOS ? name.substr(i) : "";
                try {
                    map->at(extension) += 1;
                } catch (const std::out_of_range& e) {
                    map->insert({extension, 0});
                    map->at(extension) += 1;
                }
            }
        }
    }
}

// This function is to offload some line counts away from main
// Its function is to sort a vector of pair objects using a lambda function
// and the sort function from the algorithm library.
void sortFiles(IntStrPairVec &files) {
    std::sort(files.begin(), files.end(),
            [](filePair& f1, filePair& f2) {
        if (f1.first > f2.first) {
            return true;
        } else if (f1.first < f2.first) {
            return false;
        } else {
            return (f1.second > f2.second);
        }
    });
}

// This function formats all the output to be displayed on the console.
void fileReport(std::string path, IntStrPairVec files, TypeCount map) {
    std::cout << "File Report for Directory: " << path
    << "\n--------------------------------------------------"
    << "\nFiles (sorted descending by size, then by name):" << std::endl;

    for (auto pair : files) {
        std::cout << "  " << pair.second << "  -  "
        << pair.first << " bytes" << std::endl;
    }

    std::cout << "\nFile Type Summary (sorted alphabetically):" << std::endl;

    for (auto pair : map) {
        std::string end = pair.second > 1 ? " file(s)" : " file";
        std::cout << std::setw(14) << std::right << pair.first
        << " -> " << pair.second << end << std::endl;
    }
}

int main(int argc, char* argv[]) {
    // Error if incorrect number of arguments provided
    if (argc != 2) {
        std::cerr << "Usage: homework01 <directory>" << std::endl;
        return 0;
    }

    // Uses opendir system function to open the provided directory
    std::string path = argv[1];
    auto directory = opendir(&path[0]);

    // Error if directory doesn't exist
    if (directory == NULL) {
        std::cerr << "Error: Unable to open directory " << path << std::endl;
        return 0;
    }

    // Variables to store information of files
    IntStrPairVec fileVector;
    TypeCount typeMap;

    // Make a loadVector function to reduce line count
    loadVectorAndMap(path, directory, &fileVector, &typeMap);

    sortFiles(fileVector);

    fileReport(path, fileVector, typeMap);

    closedir(directory);

    // dirInfo is the buffer where I'll store all the information
    // obtained from the stat() function about the given directory
    // struct stat dirInfo;

    // int STATUS = stat(argv[1], &dirInfo);


    return 0;
}
