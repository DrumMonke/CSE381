// Copyright 2026 Andrew Marin

#include <iostream>

int main(int argc, char *argv[]) {
    if (argc-1 != 2) {
        std::cout << "Usage: lab01 number1 number2" << std::endl;
    } else {
        std::cout << "Welcome to the Number Analyzer!" << std::endl;
        std::cout << "First number is " << argv[1] << std::endl;
        std::cout << "Second number is " << argv[2] << std::endl;
    }
    return 0;
}
