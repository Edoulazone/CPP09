/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschmitz <eschmitz@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 17:36:45 by eschmitz          #+#    #+#             */
/*   Updated: 2025/05/15 17:39:05 by eschmitz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PmergeMe.hpp"

int main(int argc, char **argv) {
    PmergeMe sorter;
    
    try {
        // Si aucun argument n'est fourni
        if (argc == 1) {
            std::cerr << "Error: No numbers provided." << std::endl;
            std::cerr << "Usage: " << argv[0] << " [positive integers...]" << std::endl;
            return 1;
        }
        
        // Parse les arguments
        if (!sorter.parseInput(argc, argv)) {
            std::cerr << "Error: Failed to parse input." << std::endl;
            return 1;
        }
        
        // Effectue le tri et affiche les résultats
        sorter.sortAndMeasure();
        
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
