/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschmitz <eschmitz@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 15:28:59 by eschmitz          #+#    #+#             */
/*   Updated: 2025/05/15 16:18:53 by eschmitz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BitcoinExchange.hpp"
#include <fstream>
#include <sstream>

// C++98 compatible string trimming helper functions
std::string& ltrim(std::string& s) {
    std::string::iterator it = s.begin();
    while (it != s.end() && (*it == ' ' || *it == '\t'))
        it++;
    s.erase(s.begin(), it);
    return s;
}

std::string& rtrim(std::string& s) {
    std::string::reverse_iterator it = s.rbegin();
    size_t count = 0;
    while (it != s.rend() && (*it == ' ' || *it == '\t')) {
        ++it;
        ++count;
    }
    if (count > 0)
        s.erase(s.size() - count);
    return s;
}

std::string& trim(std::string& s) {
    return ltrim(rtrim(s));
}

int main(int argc, char **argv) {
    // Check for correct number of arguments
    if (argc != 2) {
        std::cerr << "Error: could not open file." << std::endl;
        return 1;
    }

    // Create Bitcoin exchange database
    Bitcoin btc;
    
    // Parse the exchange rates database
    try {
        btc.parseFile("data.csv");
    } catch (std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    // Open input file
    std::ifstream input(argv[1]);
    if (!input.is_open()) {
        std::cerr << "Error: could not open file " << argv[1] << std::endl;
        return 1;
    }

    // Process input file
    std::string line;
    
    // Skip header line
    std::getline(input, line);
    
    // Process data lines
    while (std::getline(input, line)) {
        std::istringstream iss(line);
        std::string date, value;
        
        // Parse line in format "date | value"
        if (std::getline(iss, date, '|') && std::getline(iss, value)) {
            // Trim whitespace (using C++98 compatible functions)
            trim(date);
            trim(value);
            
            // Validate date
            if (!btc.isValidDate(date)) {
                std::cerr << "Error: bad input => " << line << std::endl;
                continue;
            }
            
            // Validate value
            if (!btc.isValidRate(value)) {
                std::cerr << "Error: not a positive number." << std::endl;
                continue;
            }
            
            // Parse value
            double val = std::strtod(value.c_str(), NULL);
            
            // Check if value is too large
            if (val > 1000) {
                std::cerr << "Error: too large a number." << std::endl;
                continue;
            }
            
            // Get exchange rate and calculate result
            double rate = btc.getExchangeRate(date);
            double result = val * rate;
            
            // Output result
            std::cout << date << " => " << val << " = " << result << std::endl;
        } else {
            std::cerr << "Error: bad input => " << line << std::endl;
        }
    }
    
    input.close();
    return 0;
}
