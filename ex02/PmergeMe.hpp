/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschmitz <eschmitz@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 17:36:50 by eschmitz          #+#    #+#             */
/*   Updated: 2025/08/22 15:56:48 by eschmitz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <vector>
#include <deque>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <ctime>
#include <iomanip>
#include <cstdlib>
#include <limits>
#include <utility>

class PmergeMe {
private:
    std::vector<int> _vector;  // First container for Ford-Johnson algorithm
    std::deque<int> _deque;    // Second container for Ford-Johnson algorithm
    
    // Structure for pairs used in Ford-Johnson algorithm
    typedef std::pair<int, int> Pair;
    
    // Common utility methods
    std::vector<std::string> split(const std::string& str, char delimiter);  // Split string by delimiter
    std::vector<int> generateJacobsthalSequence(int n);  // Generate Jacobsthal sequence for optimal insertion order
    int binarySearch(const std::vector<int>& arr, int value, int end);  // Binary search for vector
    int binarySearch(const std::deque<int>& arr, int value, int end);   // Binary search for deque
    
    // Ford-Johnson algorithm implementation for vector
    void fordJohnsonVector(std::vector<int>& arr);
    std::vector<int> sortPairsVector(const std::vector<std::pair<int, int> >& pairs);
    
    // Ford-Johnson algorithm implementation for deque
    void fordJohnsonDeque(std::deque<int>& arr);
    std::deque<int> sortPairsDeque(const std::deque<std::pair<int, int> >& pairs);
    
public:
    // Orthodox Canonical Form
    PmergeMe();
    PmergeMe(const PmergeMe& other);
    PmergeMe& operator=(const PmergeMe& other);
    ~PmergeMe();
    
    // Parse command line arguments into positive integers
    bool parseInput(int argc, char **argv);
    
    // Execute Ford-Johnson sort on both containers and measure time
    void sortAndMeasure();
    
    // Getters for sorted containers
    const std::vector<int>& getVector() const;
    const std::deque<int>& getDeque() const;
    
    // Exception for invalid input
    class InvalidInputException : public std::exception {
    public:
        virtual const char* what() const throw();
    };
};

#endif
