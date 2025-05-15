/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschmitz <eschmitz@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 14:02:48 by eschmitz          #+#    #+#             */
/*   Updated: 2025/05/15 14:16:20 by eschmitz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <exception>
#include <algorithm>
#include <map>
#include <string>

#define JANUARY 1
#define FEBRUARY 2
#define MARCH 3
#define APRIL 4
#define MAY 5
#define JUNE 6
#define JULY 7
#define AUGUST 8
#define SEPTEMBER 9
#define OCTOBER 10
#define NOVEMBER 11
#define DECEMBER 12

class Bitcoin {
	private:
		std::map<std::string, double>	_exchangeRates;
	
	public:
		Bitcoin();
		~Bitcoin();
		Bitcoin(const Bitcoin &copy);
		Bitcoin &operator=(const Bitcoin &copy);

		void	parseFile(const std::string &filename);
		double	getExchangeRate(const std::string &date) const;
		bool	isValidDate(const std::string &date) const;
		bool	isValidRate(const std::string &value) const;
};
