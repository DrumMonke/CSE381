// Copyright Andrew Marin 2026
// lab04.cpp
// Starter code for Lab 04: Containers and Algorithms
// Usage: ./lab04 <filename> <k>
//
// Complete TODOs in three layers:
//   - TODO(MVP)   : file I/O + total count + empty-file behavior (4 pts)
//   - TODO(NEXT)  : summary stats with <algorithm> (4 pts)
//   - TODO(FINAL) : frequency map + sorted top-k (2 pts)
//
// Constraints: no global variables; lowerCamelCase; concise functions.

#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <numeric>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

// ---------------------------------------------------------------------
// Declarations for lambda-free helpers students will implement later.
// ---------------------------------------------------------------------

// Helper predicate for count_if (lambda-free).
bool isEven(int x) { return x % 2 == 0; }

// Helper comparator for sort (lambda-free).
bool compareByFreqThenValue(const std::pair<int, std::size_t>& lhs,
                            const std::pair<int, std::size_t>& rhs) {
    if (lhs.second > rhs.second) {
      return true;
    } else if (lhs.second < rhs.second) {
      return false;
    } else {
      return lhs.first < rhs.first;
    }
  }


// ---------------------------------------------------------------------
// MVP: parsing helpers and file loading
// ---------------------------------------------------------------------

// Reads a single line, extracts all integers, and appends them to `out`.
// Lines beginning with '#' are comments. Non-integer tokens are skipped.
void parseIntsFromLine(const std::string& line, std::vector<int>& out) {
    // Checks for empty lines or comments
    if (line.empty() || line[0] == '#') return;

    std::stringstream ss(line);
    std::string part;

    while (ss >> part) {
        // Try string to int conversion
        try {
            int num = std::stoi(part);
            out.push_back(num);
        } catch (std::exception& e) {}
    }
}

// Read all integers from a file into a vector<int>. Throw on open failure.
std::vector<int> loadNumbers(const std::string& path) {
  std::vector<int> out;
  std::fstream fs(path);
  if (!fs.is_open()) {
    throw std::runtime_error("Failed to open: " + path);
  }

  std::string line;
  while (std::getline(fs, line)) {
    parseIntsFromLine(line, out);
  }

  return out;
}

// ---------------------------------------------------------------------
// NEXT: summary statistics with <algorithm>
// ---------------------------------------------------------------------

// Return the count of unique values using sort + unique + erase.
std::size_t computeUniqueCount(const std::vector<int>& a) {
  std::vector<int> copy(a);
  std::sort(copy.begin(), copy.end());
  auto unique = std::unique(copy.begin(), copy.end());
  copy.erase(unique, copy.end());

  return copy.size();
}

// Return the average as double using accumulate (64-bit sum). Empty -> 0.0.
double computeAverage(const std::vector<int>& a) {
  std::int64_t initial = 0;
  std::int64_t sum = std::accumulate(a.begin(), a.end(), initial);
  return static_cast<double>(sum) / a.size();
}

// Return how many elements are even using std::count_if and isEven().
std::size_t countEvens(const std::vector<int>& a) {
  return std::count_if(a.begin(), a.end(), isEven);
}

// ---------------------------------------------------------------------
// FINAL: frequency table + sorted top-k
// ---------------------------------------------------------------------

// Build a frequency table: value -> count using std::unordered_map.
std::unordered_map<int, std::size_t> buildFreq(const std::vector<int>& a) {
  std::unordered_map<int, std::size_t> umap;
  for (auto val : a) {
    umap[val]++;
  }
  return umap;
}

// Convert the map to vector<pair<int,size_t>> and sort with comparator.
// f is the unordered map
std::vector<std::pair<int, std::size_t>>
sortedItemsByFreq(const std::unordered_map<int, std::size_t>& f) {
  std::vector<std::pair<int, std::size_t>> items(f.begin(), f.end());
  std::sort(items.begin(), items.end(), compareByFreqThenValue);
  return items;
}

// Print the top-k entries "value -> count". If fewer than k, print them all.
void printTopK(const std::vector<std::pair<int, std::size_t>>& items,
               std::size_t k) {
  std::size_t limit = (k < items.size()) ? k : items.size();
  std::cout << "Top-" << k << ":\n";
  for (std::size_t i = 0; i < limit; i++) {
    std::cout << items[i].first << " -> " << items[i].second << std::endl;
  }
}

// ---------------------------------------------------------------------
// Printing utilities / control flow
// ---------------------------------------------------------------------

void printUsage() {
  std::cerr << "Usage: ./lab04 <filename> <k>\n";
}

// Print the "empty file" summary (used in MVP and beyond).
void printEmptySummary(std::size_t k) {
  std::cout << "Total: 0" << std::endl
            << "Unique: 0" << std::endl
            << "Average: 0" << std::endl
            << "Min: n/a" << std::endl
            << "Max: n/a" << std::endl
            << "Even numbers: 0" << std::endl
            << "\nTop-" << k << ":" << std::endl;
}

// Compute stats and print summary + (eventually) top-k list.
// Use: sort/unique, accumulate, min_element, max_element, count_if.
void printSummary(const std::vector<int>& nums, std::size_t k) {
  std::cout << "Total: " << nums.size() << std::endl;
  std::cout << "Unique: " << computeUniqueCount(nums) << std::endl;
  std::cout << "Average: " << computeAverage(nums) << std::endl;
  auto min_elem = *std::min_element(nums.begin(), nums.end());
  auto max_elem = *std::max_element(nums.begin(), nums.end());
  std::cout << "Min: " << min_elem << std::endl;
  std::cout << "Max: " << max_elem << std::endl;
  std::cout << "Even numbers: " << countEvens(nums) << std::endl;
  std::cout << std::endl;
  //
  // TODO(FINAL):
  //   - Build frequency map (buildFreq)
  //   - Transform + sort (sortedItemsByFreq)
  //   - Print top-k list (printTopK)
  std::unordered_map<int, std::size_t> umap = buildFreq(nums);
  std::vector<std::pair<int, std::size_t>> items = sortedItemsByFreq(umap);
  printTopK(items, k);

  (void)nums;  // remove after implementing
  (void)k;     // remove after implementing
}

// Orchestrate parsing, empty-input handling, and summary printing.
int run(int argc, char* argv[]) {
  if (argc != 3) {
    printUsage();
    return 0;
  }
  const std::string fileName = argv[1];
  const std::size_t k = static_cast<std::size_t>(std::stoull(argv[2]));

  std::fstream fs(fileName);

  // loadNumbers will throw file opening error
  std::vector<int> nums =  loadNumbers(fileName);
  if (nums.empty()) {
      printEmptySummary(k);
  } else {
      printSummary(nums, k);
  }
  return 0;
}

int main(int argc, char* argv[]) {
  try {
    return run(argc, argv);
  } catch (const std::exception& e) {
    std::cerr << "error: " << e.what() << "\n";
    return 0;
  }
}
