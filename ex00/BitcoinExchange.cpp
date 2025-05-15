/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschmitz <eschmitz@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 15:28:54 by eschmitz          #+#    #+#             */
/*   Updated: 2025/05/15 14:16:09 by eschmitz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BitcoinExchange.hpp"

Bitcoin::Bitcoin(){
}

Bitcoin::~Bitcoin() {
}

Bitcoin::Bitcoin(const Bitcoin &copy): _exchangeRates(copy._exchangeRates) {
}

Bitcoin &Bitcoin::operator=(const Bitcoin &copy) {
	_exchangeRates = copy._exchangeRates;
	return *this;
}

bool	Bitcoin::isValidDate(const std::string &date) const {
	unsigned int year, month, day;
}

bool	Bitcoin::isValidRate(const std::string &value) const {

}

void	Bitcoin::parseFile(const std::string &fileName) {

}

double	Bitcoin::getExchangeRate(const std::string &date) const {

}