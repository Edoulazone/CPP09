/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschmitz <eschmitz@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 17:36:48 by eschmitz          #+#    #+#             */
/*   Updated: 2025/08/22 15:57:42 by eschmitz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PmergeMe.hpp"

// Orthodox Canonical Form implementation
PmergeMe::PmergeMe() {}

PmergeMe::PmergeMe(const PmergeMe& other) : _vector(other._vector), _deque(other._deque) {}

PmergeMe& PmergeMe::operator=(const PmergeMe& other) {
    if (this != &other) {
        _vector = other._vector;
        _deque = other._deque;
    }
    return *this;
}

PmergeMe::~PmergeMe() {}

// Split string by delimiter (space by default)
std::vector<std::string> PmergeMe::split(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;
    
    while (std::getline(ss, token, delimiter)) {
        // Remove leading and trailing whitespace
        size_t start = token.find_first_not_of(" \t\n\r");
        if (start == std::string::npos) continue; // Skip empty tokens
        
        size_t end = token.find_last_not_of(" \t\n\r");
        token = token.substr(start, end - start + 1);
        
        if (!token.empty()) {
            tokens.push_back(token);
        }
    }
    
    return tokens;
}

// Parse command line arguments and validate input
bool PmergeMe::parseInput(int argc, char **argv) {
    if (argc < 2)
        return false;
    
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        
        // Split argument by spaces to handle multiple numbers in one argument
        std::vector<std::string> numbers = split(arg, ' ');
        
        // If no split occurred, treat as single number
        if (numbers.empty()) {
            numbers.push_back(arg);
        }
        
        // Process each number found in the argument
        for (size_t j = 0; j < numbers.size(); j++) {
            std::string numberStr = numbers[j];
            
            // Skip empty strings
            if (numberStr.empty()) continue;
            
            // Check if number string contains only digits (positive integer)
            for (size_t k = 0; k < numberStr.length(); k++) {
                if (!isdigit(numberStr[k]))
                    throw InvalidInputException();
            }
            
            // Convert to integer and check for overflow
            char* endptr;
            long value = std::strtol(numberStr.c_str(), &endptr, 10);
            if (value > std::numeric_limits<int>::max() || value <= 0)
                throw InvalidInputException();
            
            // Add to both containers for comparison
            _vector.push_back(static_cast<int>(value));
            _deque.push_back(static_cast<int>(value));
        }
    }
    return true;
}

// Generate Jacobsthal sequence: t_k = 2^k - t_{k-1}
// This sequence determines the optimal insertion order in Ford-Johnson algorithm
std::vector<int> PmergeMe::generateJacobsthalSequence(int n) {
    std::vector<int> jacobsthal;
    
    if (n <= 0) return jacobsthal;
    
    // Start with first Jacobsthal numbers: 1, 3
    jacobsthal.push_back(1);
    if (n <= 1) return jacobsthal;
    
    jacobsthal.push_back(3);
    
    // Generate sequence: 1, 3, 5, 11, 21, 43, 85, ...
    while (true) {
        int next = 2 * jacobsthal[jacobsthal.size() - 1] + jacobsthal[jacobsthal.size() - 2];
        if (next > n) break;
        jacobsthal.push_back(next);
    }
    
    return jacobsthal;
}

// Binary search implementation for vector container
// Used to find optimal insertion position with minimal comparisons
int PmergeMe::binarySearch(const std::vector<int>& arr, int value, int end) {
    int left = 0;
    int right = end;
    
    while (left < right) {
        int mid = left + (right - left) / 2;
        if (arr[mid] < value) {
            left = mid + 1;
        } else {
            right = mid;
        }
    }
    return left;
}

// Binary search implementation for deque container
int PmergeMe::binarySearch(const std::deque<int>& arr, int value, int end) {
    int left = 0;
    int right = end;
    
    while (left < right) {
        int mid = left + (right - left) / 2;
        if (arr[mid] < value) {
            left = mid + 1;
        } else {
            right = mid;
        }
    }
    return left;
}

// Ford-Johnson merge-insertion sort algorithm for vector
void PmergeMe::fordJohnsonVector(std::vector<int>& arr) {
    if (arr.size() <= 1) return;
    
    // STEP 1: Create pairs and sort them (larger element first)
    std::vector<std::pair<int, int> > pairs;
    bool hasOdd = false;
    int oddElement = 0;
    
    // Form pairs from consecutive elements
    for (size_t i = 0; i + 1 < arr.size(); i += 2) {
        if (arr[i] > arr[i + 1]) {
            pairs.push_back(std::make_pair(arr[i], arr[i + 1]));  // (larger, smaller)
        } else {
            pairs.push_back(std::make_pair(arr[i + 1], arr[i]));  // (larger, smaller)
        }
    }
    
    // Handle odd element if present
    if (arr.size() % 2 == 1) {
        hasOdd = true;
        oddElement = arr[arr.size() - 1];
    }
    
    // STEP 2: Recursively sort the larger elements
    // This creates the foundation of our main chain
    std::vector<int> largerElements = sortPairsVector(pairs);
    
    // STEP 3: Build main chain starting with smallest of first pair
    std::vector<int> mainChain;
    std::vector<int> toInsert;
    
    // Add the smallest element of the first pair (it's guaranteed to be the smallest overall)
    if (!pairs.empty()) {
        mainChain.push_back(pairs[0].second);
    }
    
    // Add all larger elements to main chain (already sorted)
    for (size_t i = 0; i < largerElements.size(); i++) {
        mainChain.push_back(largerElements[i]);
    }
    
    // Prepare smaller elements for insertion (skip first pair's smaller element)
    for (size_t i = 1; i < pairs.size(); i++) {
        toInsert.push_back(pairs[i].second);
    }
    
    // STEP 4: Insert elements using Jacobsthal sequence for optimal order
    std::vector<int> jacobsthal = generateJacobsthalSequence(toInsert.size());
    std::vector<bool> inserted(toInsert.size(), false);
    
    // Insert according to Jacobsthal sequence
    for (size_t i = 0; i < jacobsthal.size(); i++) {
        int idx = jacobsthal[i] - 1; // Convert to 0-based index
        
        // Insert in decreasing order from current Jacobsthal number to previous one
        int prevJacobsthal = (i > 0) ? jacobsthal[i - 1] : 0;
        
        for (int j = std::min(idx, (int)toInsert.size() - 1); j >= prevJacobsthal; j--) {
            if (j < (int)toInsert.size() && !inserted[j]) {
                // Use binary search to find optimal insertion position
                int pos = binarySearch(mainChain, toInsert[j], mainChain.size());
                mainChain.insert(mainChain.begin() + pos, toInsert[j]);
                inserted[j] = true;
            }
        }
    }
    
    // Insert any remaining elements
    for (size_t i = 0; i < toInsert.size(); i++) {
        if (!inserted[i]) {
            int pos = binarySearch(mainChain, toInsert[i], mainChain.size());
            mainChain.insert(mainChain.begin() + pos, toInsert[i]);
        }
    }
    
    // Insert odd element if it exists
    if (hasOdd) {
        int pos = binarySearch(mainChain, oddElement, mainChain.size());
        mainChain.insert(mainChain.begin() + pos, oddElement);
    }
    
    arr = mainChain;
}

// Recursively sort the larger elements of pairs
std::vector<int> PmergeMe::sortPairsVector(const std::vector<std::pair<int, int> >& pairs) {
    if (pairs.size() <= 1) {
        std::vector<int> result;
        if (!pairs.empty()) {
            result.push_back(pairs[0].first);
        }
        return result;
    }
    
    // Extract larger elements from all pairs
    std::vector<int> largerElements;
    for (size_t i = 0; i < pairs.size(); i++) {
        largerElements.push_back(pairs[i].first);
    }
    
    // Recursively sort them using Ford-Johnson
    fordJohnsonVector(largerElements);
    
    return largerElements;
}

// Ford-Johnson merge-insertion sort algorithm for deque (same logic as vector)
void PmergeMe::fordJohnsonDeque(std::deque<int>& arr) {
    if (arr.size() <= 1) return;
    
    // STEP 1: Create pairs and sort them (larger element first)
    std::deque<std::pair<int, int> > pairs;
    bool hasOdd = false;
    int oddElement = 0;
    
    // Form pairs from consecutive elements
    for (size_t i = 0; i + 1 < arr.size(); i += 2) {
        if (arr[i] > arr[i + 1]) {
            pairs.push_back(std::make_pair(arr[i], arr[i + 1]));
        } else {
            pairs.push_back(std::make_pair(arr[i + 1], arr[i]));
        }
    }
    
    // Handle odd element if present
    if (arr.size() % 2 == 1) {
        hasOdd = true;
        oddElement = arr[arr.size() - 1];
    }
    
    // STEP 2: Recursively sort the larger elements
    std::deque<int> largerElements = sortPairsDeque(pairs);
    
    // STEP 3: Build main chain starting with smallest of first pair
    std::deque<int> mainChain;
    std::deque<int> toInsert;
    
    // Add the smallest element of the first pair
    if (!pairs.empty()) {
        mainChain.push_back(pairs[0].second);
    }
    
    // Add all larger elements to main chain
    for (size_t i = 0; i < largerElements.size(); i++) {
        mainChain.push_back(largerElements[i]);
    }
    
    // Prepare smaller elements for insertion
    for (size_t i = 1; i < pairs.size(); i++) {
        toInsert.push_back(pairs[i].second);
    }
    
    // STEP 4: Insert elements using Jacobsthal sequence
    std::vector<int> jacobsthal = generateJacobsthalSequence(toInsert.size());
    std::vector<bool> inserted(toInsert.size(), false);
    
    for (size_t i = 0; i < jacobsthal.size(); i++) {
        int idx = jacobsthal[i] - 1;
        
        int prevJacobsthal = (i > 0) ? jacobsthal[i - 1] : 0;
        
        for (int j = std::min(idx, (int)toInsert.size() - 1); j >= prevJacobsthal; j--) {
            if (j < (int)toInsert.size() && !inserted[j]) {
                int pos = binarySearch(mainChain, toInsert[j], mainChain.size());
                mainChain.insert(mainChain.begin() + pos, toInsert[j]);
                inserted[j] = true;
            }
        }
    }
    
    // Insert any remaining elements
    for (size_t i = 0; i < toInsert.size(); i++) {
        if (!inserted[i]) {
            int pos = binarySearch(mainChain, toInsert[i], mainChain.size());
            mainChain.insert(mainChain.begin() + pos, toInsert[i]);
        }
    }
    
    // Insert odd element if it exists
    if (hasOdd) {
        int pos = binarySearch(mainChain, oddElement, mainChain.size());
        mainChain.insert(mainChain.begin() + pos, oddElement);
    }
    
    arr = mainChain;
}

// Recursively sort the larger elements of pairs for deque
std::deque<int> PmergeMe::sortPairsDeque(const std::deque<std::pair<int, int> >& pairs) {
    if (pairs.size() <= 1) {
        std::deque<int> result;
        if (!pairs.empty()) {
            result.push_back(pairs[0].first);
        }
        return result;
    }
    
    // Extract larger elements from all pairs
    std::deque<int> largerElements;
    for (size_t i = 0; i < pairs.size(); i++) {
        largerElements.push_back(pairs[i].first);
    }
    
    // Recursively sort them using Ford-Johnson
    fordJohnsonDeque(largerElements);
    
    return largerElements;
}

// Execute sorting algorithm and measure performance
void PmergeMe::sortAndMeasure() {
    // Display unsorted sequence
    std::cout << "Before: ";
    for (size_t i = 0; i < _vector.size(); i++) {
        std::cout << _vector[i] << " ";
    }
    std::cout << std::endl;
    
    // Measure time for vector container
    clock_t start_vector = clock();
    fordJohnsonVector(_vector);
    clock_t end_vector = clock();
    double time_vector = static_cast<double>(end_vector - start_vector) / CLOCKS_PER_SEC * 1000000;
    
    // Measure time for deque container
    clock_t start_deque = clock();
    fordJohnsonDeque(_deque);
    clock_t end_deque = clock();
    double time_deque = static_cast<double>(end_deque - start_deque) / CLOCKS_PER_SEC * 1000000;
    
    // Display sorted sequence
    std::cout << "After: ";
    for (size_t i = 0; i < _vector.size(); i++) {
        std::cout << _vector[i] << " ";
    }
    std::cout << std::endl;
    
    // Display performance metrics
    std::cout << "Time to process a range of " << _vector.size() << " elements with std::vector : " 
              << std::fixed << std::setprecision(5) << time_vector << " us" << std::endl;
    std::cout << "Time to process a range of " << _deque.size() << " elements with std::deque : " 
              << std::fixed << std::setprecision(5) << time_deque << " us" << std::endl;
}

// Getter methods
const std::vector<int>& PmergeMe::getVector() const {
    return _vector;
}

const std::deque<int>& PmergeMe::getDeque() const {
    return _deque;
}

// Exception implementation
const char* PmergeMe::InvalidInputException::what() const throw() {
    return "Error: Invalid input (must be positive integers only)";
}
