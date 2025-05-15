/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschmitz <eschmitz@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 17:22:07 by eschmitz          #+#    #+#             */
/*   Updated: 2025/05/15 17:23:16 by eschmitz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>
#include "RPN.hpp"

int main(int argc, char **argv) {
    // Check if we have exactly one argument
    if (argc != 2) {
        std::cerr << "Error: Program requires exactly one argument" << std::endl;
        std::cerr << "Usage: " << argv[0] << " \"RPN expression\"" << std::endl;
        return 1;
    }

    // Get the expression from command line
    std::string expression = argv[1];
    
    // Create RPN calculator
    RPN calculator;
    
    try {
        // Calculate the result
        int result = calculator.calculate(expression);
        
        // Output the result
        std::cout << result << std::endl;
    }
    catch (const RPNException& e) {
        // Output error message to standard error
        std::cerr << e.what() << std::endl;
        return 1;
    }
    catch (const std::exception& e) {
        // Handle any other standard exceptions
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
