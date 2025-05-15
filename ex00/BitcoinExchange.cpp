/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschmitz <eschmitz@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 15:28:54 by eschmitz          #+#    #+#             */
/*   Updated: 2025/05/15 16:18:51 by eschmitz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BitcoinExchange.hpp"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cstdlib>
#include <cstdio>

Bitcoin::Bitcoin(){
}

Bitcoin::~Bitcoin() {
}

Bitcoin::Bitcoin(const Bitcoin &copy): _exchangeRates(copy._exchangeRates) {
}

Bitcoin &Bitcoin::operator=(const Bitcoin &copy) {
    if (this != &copy)
        _exchangeRates = copy._exchangeRates;
    return *this;
}

bool Bitcoin::isValidDate(const std::string &date) const {
    // Check date format: YYYY-MM-DD
    if (date.length() != 10 || date[4] != '-' || date[7] != '-')
        return false;
    
    // Parse year, month, and day
    unsigned int year, month, day;
    // C++98 compatible string to int conversion
    std::istringstream yearStream(date.substr(0, 4));
    std::istringstream monthStream(date.substr(5, 2));
    std::istringstream dayStream(date.substr(8, 2));
    
    if (!(yearStream >> year) || !(monthStream >> month) || !(dayStream >> day))
        return false;
    
    // Validate month
    if (month < 1 || month > 12)
        return false;
    
    // Validate day based on month and leap year
    unsigned int daysInMonth;
    switch (month) {
        case FEBRUARY:
            // Check for leap year
            if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)
                daysInMonth = 29;
            else
                daysInMonth = 28;
            break;
        case APRIL:
        case JUNE:
        case SEPTEMBER:
        case NOVEMBER:
            daysInMonth = 30;
            break;
        default:
            daysInMonth = 31;
    }
    
    return day >= 1 && day <= daysInMonth;
}

bool Bitcoin::isValidRate(const std::string &value) const {
    char *end;
    double val = strtod(value.c_str(), &end);
    
    if (end != value.c_str() && *end == '\0' && val >= 0)
        return true;
    return false;
}

void Bitcoin::parseFile(const std::string &fileName) {
    std::ifstream file(fileName.c_str());
    
    if (!file.is_open()) {
        std::cerr << "Error: could not open file " << fileName << std::endl;
        return;
    }
    
    std::string line;
    // Skip header line
    if (!std::getline(file, line))
        return;
    
    // Read data lines
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string date;
        std::string rate;
        
        // Parse CSV format (date,exchange_rate)
        if (std::getline(iss, date, ',') && std::getline(iss, rate)) {
            if (isValidDate(date) && isValidRate(rate))
                _exchangeRates[date] = std::strtod(rate.c_str(), NULL);
            else
                std::cerr << "Warning: invalid data in CSV: " << line << std::endl;
        }
    }
    
    file.close();
}

double Bitcoin::getExchangeRate(const std::string &date) const {
    // Find the closest date that is not after the given date
    std::map<std::string, double>::const_iterator it = _exchangeRates.upper_bound(date);
    
    if (it == _exchangeRates.begin()) {
        // No exchange rate found before the given date
        return 0;
    }
    
    // Return the exchange rate for the closest prior date
    --it;
    return it->second;
}
