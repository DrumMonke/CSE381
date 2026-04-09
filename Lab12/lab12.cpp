// Copyright (c) 2026 Andrew Marin

#include <unistd.h>
#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>
#include <thread>
#include <mutex>


// Base amount of time (in microseconds) to sleep for transactions
// You can reduce this number during development but you will lose
// points if you forget to set it back to 300000 when you submit it
#define SLEEPBASE 300000

// Some aliases and shortcuts to simplify the code
using std::cout, std::endl, std::string;
using StrFltMap = std::map<std::string, float>;
using StrMuteMap = std::unordered_map<std::string, std::mutex>;

// Function to deposit money into an account.  An actual deposit
// would be more complex than just updating a vector (e.g. doing
// checks, retrieving data over the network, etc.) so a usleep
// statement is put into the function to mimic that.
void deposit(string acc, StrFltMap& account, StrMuteMap& mutexMap, float amt) {
    std::lock_guard<std::mutex> lock(mutexMap[acc]);
    account[acc] += amt;
    usleep(SLEEPBASE);
}

// Function to withdraw money from an account.  An actual withdraw
// would be more complex than just updating a vector (e.g. doing
// checks, retrieving data over the network, etc.) so a usleep
// statement is put into the function to mimic that.
// Note that before the withdraw is processed, the account balance
// must be checked to make sure there are sufficient funds.
void withdraw(string acc, StrFltMap& account, StrMuteMap& mutexMap, float amt) {
    std::lock_guard<std::mutex> lock(mutexMap[acc]);
    if (account[acc] >= amt) {
        account[acc] -= amt;
        usleep(SLEEPBASE);
    } else {
        std::cerr << "Insufficient funds to withdraw $" << amt << endl;
    }
}

// Function to transfer money from one account to another.  An actual transfer
// would be more complex than just updating a vector (e.g. doing
// checks, retrieving data over the network, etc.) so a usleep
// statement is put into the function to mimic that.
// Note that before the withdraw from the "from" is processed, the account
// balance must be checked to make sure there are sufficient funds.
void transfer(string from, string to, StrFltMap& account,
    StrMuteMap& mutexMap, float amt) {
    std::scoped_lock lock(mutexMap[from], mutexMap[to]);
    if (account[from] >= amt) {
        account[from] -= amt;
        account[to] += amt;
        usleep(2*SLEEPBASE);
    } else {
        std::cerr << "Insufficient funds to transfer $" << amt << endl;
    }
}

// Initialize the account balances
void initializeAccounts(StrFltMap& accts) {
    accts["acct000"] = 3700.00;
    accts["acct001"] = 1800.00;
    accts["acct002"] = 3300.00;
    accts["acct003"] = 6500.00;
    accts["acct004"] = 5000.00;
}

// A function that processes all of the transactions, updating the vector with
// the account balances via the reference.  The same set of transactions is
// repeated with different amounts.
void processTransactions(StrFltMap& acct, StrMuteMap& mutexMap) {
    std::vector<float> transactions = {23, 110, 75, 119, 280, 250, 140, 37};
    std::vector<std::thread> threads;

    for (float amt : transactions) {
        // Do this for each transaction, make sure to pass the mutex list in
        threads.push_back(std::thread(deposit, "acct000", std::ref(acct),
                          std::ref(mutexMap), amt));

        threads.push_back(std::thread(transfer, "acct001", "acct002",
                          std::ref(acct), std::ref(mutexMap), amt));

        threads.push_back(std::thread(withdraw, "acct003", std::ref(acct),
                          std::ref(mutexMap), amt));

        threads.push_back(std::thread(transfer, "acct004", "acct003",
                          std::ref(acct), std::ref(mutexMap), amt));

        threads.push_back(std::thread(deposit, "acct004", std::ref(acct),
                          std::ref(mutexMap), amt));

        threads.push_back(std::thread(withdraw, "acct000", std::ref(acct),
                          std::ref(mutexMap), amt));

        threads.push_back(std::thread(transfer, "acct002", "acct000",
                          std::ref(acct), std::ref(mutexMap), amt));

        threads.push_back(std::thread(transfer, "acct004", "acct001",
                          std::ref(acct), std::ref(mutexMap), amt));

        threads.push_back(std::thread(deposit, "acct001", std::ref(acct),
                          std::ref(mutexMap), amt));

        threads.push_back(std::thread(withdraw, "acct003", std::ref(acct),
                          std::ref(mutexMap), amt));
    }
    for (auto& t : threads)
        t.join();}

void fillMutexMap(StrFltMap& accts, StrMuteMap& mutexMap) {
    for (auto& m : accts) {
        // Uses the account names as the keys for the mutex map
        mutexMap[m.first];
    }
}

int main() {
    // Create a vector to hold the account balances, then initialize it.  The
    // index of the vector will serve as the account number in this simple
    // example.
    StrFltMap accts;
    StrMuteMap mutexMap;
    initializeAccounts(accts);
    fillMutexMap(accts, mutexMap);

    // Process the transactions
    processTransactions(accts, mutexMap);

    // Print out the final account balances
    cout << "Final balances\n";
    for (auto& m : accts) {
        cout << m.first << " $" << m.second << endl;
    }

    return 0;
}
