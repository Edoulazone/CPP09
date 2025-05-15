/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschmitz <eschmitz@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 17:36:50 by eschmitz          #+#    #+#             */
/*   Updated: 2025/05/15 17:39:11 by eschmitz         ###   ########.fr       */
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

class PmergeMe {
private:
    std::vector<int> _vector;
    std::deque<int> _deque;
    
    // Méthodes pour le tri avec vector
    void mergeInsertSortVector(std::vector<int>& arr, int start, int end);
    void insertSortVector(std::vector<int>& arr, int start, int end);
    void mergeVector(std::vector<int>& arr, int start, int mid, int end);
    
    // Méthodes pour le tri avec deque
    void mergeInsertSortDeque(std::deque<int>& arr, int start, int end);
    void insertSortDeque(std::deque<int>& arr, int start, int end);
    void mergeDeque(std::deque<int>& arr, int start, int mid, int end);
    
public:
    PmergeMe();
    PmergeMe(const PmergeMe& other);
    PmergeMe& operator=(const PmergeMe& other);
    ~PmergeMe();
    
    // Parse les arguments
    bool parseInput(int argc, char **argv);
    
    // Effectue le tri et mesure le temps
    void sortAndMeasure();
    
    // Getters
    const std::vector<int>& getVector() const;
    const std::deque<int>& getDeque() const;
    
    // Exception
    class InvalidInputException : public std::exception {
    public:
        virtual const char* what() const throw();
    };
};

#endif
