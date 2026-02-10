// Copyright 2026 Andrew Marin

#include <iostream>
#include <vector>

bool checkArgs(int argc, char *argv[]) {
    try {
        for (int i = 1; i < argc; i++) {
            std::stoi(argv[i]);
        }
    } catch(const std::exception& e) {
        return false;
    }
    return true;
}

int main(int argc, char *argv[]) {
    if (argc-1 != 2) {
        std::cout << "Usage: lab01 number1 number2" << std::endl;
    } else if (checkArgs(argc, argv)) {
        int num1 = std::stoi(argv[1]);
        int num2 = std::stoi(argv[2]);
        std::cout << "Welcome to the Number Analyzer!" << std::endl;
        std::cout << "First number is " << num1 << std::endl;
        std::cout << "Second number is " << num2 << std::endl;
        std::cout << "Sum is " << num1 + num2 << std::endl;
    } else {
        std::cout << "Error: arguments must be integers" << std::endl;
    }
    return 0;
}
