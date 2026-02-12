// Copyright Andrew Marin 2026

#include <iostream>
#include <string>
#include <cstdint>
#include <iomanip>

struct Person {
    std::string name;
    std::string position;
    std::string idNum;
    std::uint32_t balance;
};

void output(Person p) {
    int numSpace = 18;

    std::cout << std::setw(numSpace) << std::left << "Name:";
    std::cout << std::left << p.name << std::endl;

    std::cout << std::setw(numSpace) << std::left << "Position:";
    std::cout << std::left << p.position << std::endl;

    std::cout << std::setw(numSpace) << std::left << "ID Number:";
    std::cout << std::left << p.idNum << std::endl;

    std::cout << std::setw(numSpace) << std::left << "Vacation Balance:";
    std::cout << std::left << p.balance << std::endl;

    std::cout << std::endl;
}

int main() {
    Person p1{"Hubert J. Farnsworth", "Professor", "B0SS", 2000};

    output(p1);

    Person p2{.name = "Bender B. Rodriguez",
        .position = "Bender", .idNum = "BMSMA", .balance = 0};

    output(p2);

    Person p3;

    p3.name = "Turanga Leela";
    p3.position = "Spaceship Pilot";
    p3.idNum = "1BDI";
    p3.balance = 1234;

    output(p3);

    return 0;
}
