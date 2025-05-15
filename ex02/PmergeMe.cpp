/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschmitz <eschmitz@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 17:36:48 by eschmitz          #+#    #+#             */
/*   Updated: 2025/05/15 17:39:09 by eschmitz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PmergeMe.hpp"

// Constructeurs et destructeur
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

// Méthode pour parser les entrées
bool PmergeMe::parseInput(int argc, char **argv) {
    if (argc < 2)
        return false;
    
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        
        // Vérifier que l'argument est un nombre positif
        for (size_t j = 0; j < arg.length(); j++) {
            if (!isdigit(arg[j]))
                throw InvalidInputException();
        }
        
        // Convertir en entier et vérifier les dépassements
        char* endptr;
        long value = std::strtol(arg.c_str(), &endptr, 10);
        if (value > std::numeric_limits<int>::max())
            throw InvalidInputException();
        
        // Ajouter aux deux containers
        _vector.push_back(static_cast<int>(value));
        _deque.push_back(static_cast<int>(value));
    }
    return true;
}

// Méthodes pour le tri avec vector
void PmergeMe::insertSortVector(std::vector<int>& arr, int start, int end) {
    for (int i = start + 1; i <= end; i++) {
        int key = arr[i];
        int j = i - 1;
        
        while (j >= start && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

void PmergeMe::mergeVector(std::vector<int>& arr, int start, int mid, int end) {
    int n1 = mid - start + 1;
    int n2 = end - mid;
    
    std::vector<int> left;
    std::vector<int> right;
    
    // Réserver l'espace pour éviter les réallocations
    left.reserve(n1);
    right.reserve(n2);
    
    for (int i = 0; i < n1; i++)
        left.push_back(arr[start + i]);
    for (int j = 0; j < n2; j++)
        right.push_back(arr[mid + 1 + j]);
    
    int i = 0, j = 0, k = start;
    
    while (i < n1 && j < n2) {
        if (left[i] <= right[j]) {
            arr[k] = left[i];
            i++;
        } else {
            arr[k] = right[j];
            j++;
        }
        k++;
    }
    
    while (i < n1) {
        arr[k] = left[i];
        i++;
        k++;
    }
    
    while (j < n2) {
        arr[k] = right[j];
        j++;
        k++;
    }
}

void PmergeMe::mergeInsertSortVector(std::vector<int>& arr, int start, int end) {
    if (end - start < 10) {
        insertSortVector(arr, start, end);
        return;
    }
    
    if (start < end) {
        int mid = start + (end - start) / 2;
        
        mergeInsertSortVector(arr, start, mid);
        mergeInsertSortVector(arr, mid + 1, end);
        
        mergeVector(arr, start, mid, end);
    }
}

// Méthodes pour le tri avec deque
void PmergeMe::insertSortDeque(std::deque<int>& arr, int start, int end) {
    for (int i = start + 1; i <= end; i++) {
        int key = arr[i];
        int j = i - 1;
        
        while (j >= start && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

void PmergeMe::mergeDeque(std::deque<int>& arr, int start, int mid, int end) {
    int n1 = mid - start + 1;
    int n2 = end - mid;
    
    std::deque<int> left;
    std::deque<int> right;
    
    for (int i = 0; i < n1; i++)
        left.push_back(arr[start + i]);
    for (int j = 0; j < n2; j++)
        right.push_back(arr[mid + 1 + j]);
    
    int i = 0, j = 0, k = start;
    
    while (i < n1 && j < n2) {
        if (left[i] <= right[j]) {
            arr[k] = left[i];
            i++;
        } else {
            arr[k] = right[j];
            j++;
        }
        k++;
    }
    
    while (i < n1) {
        arr[k] = left[i];
        i++;
        k++;
    }
    
    while (j < n2) {
        arr[k] = right[j];
        j++;
        k++;
    }
}

void PmergeMe::mergeInsertSortDeque(std::deque<int>& arr, int start, int end) {
    if (end - start < 10) {
        insertSortDeque(arr, start, end);
        return;
    }
    
    if (start < end) {
        int mid = start + (end - start) / 2;
        
        mergeInsertSortDeque(arr, start, mid);
        mergeInsertSortDeque(arr, mid + 1, end);
        
        mergeDeque(arr, start, mid, end);
    }
}

// Effectue le tri et mesure le temps
void PmergeMe::sortAndMeasure() {
    // Affichage des nombres avant le tri
    std::cout << "Before: ";
    for (size_t i = 0; i < _vector.size(); i++) {
        std::cout << _vector[i] << " ";
    }
    std::cout << std::endl;
    
    // Mesure du temps pour vector
    clock_t start_vector = clock();
    mergeInsertSortVector(_vector, 0, static_cast<int>(_vector.size()) - 1);
    clock_t end_vector = clock();
    double time_vector = static_cast<double>(end_vector - start_vector) / CLOCKS_PER_SEC * 1000000;
    
    // Mesure du temps pour deque
    clock_t start_deque = clock();
    mergeInsertSortDeque(_deque, 0, static_cast<int>(_deque.size()) - 1);
    clock_t end_deque = clock();
    double time_deque = static_cast<double>(end_deque - start_deque) / CLOCKS_PER_SEC * 1000000;
    
    // Affichage des nombres après le tri
    std::cout << "After: ";
    for (size_t i = 0; i < _vector.size(); i++) {
        std::cout << _vector[i] << " ";
    }
    std::cout << std::endl;
    
    // Affichage des temps de traitement
    std::cout << "Time to process a range of " << _vector.size() << " elements with std::vector : " 
              << std::fixed << std::setprecision(5) << time_vector << " us" << std::endl;
    std::cout << "Time to process a range of " << _deque.size() << " elements with std::deque : " 
              << std::fixed << std::setprecision(5) << time_deque << " us" << std::endl;
}

// Getters
const std::vector<int>& PmergeMe::getVector() const {
    return _vector;
}

const std::deque<int>& PmergeMe::getDeque() const {
    return _deque;
}

// Exception
const char* PmergeMe::InvalidInputException::what() const throw() {
    return "Error: Invalid input (must be positive integers only)";
}
