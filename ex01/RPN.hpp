/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPN.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschmitz <eschmitz@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 17:22:01 by eschmitz          #+#    #+#             */
/*   Updated: 2025/08/22 15:14:30 by eschmitz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RPN_HPP
# define RPN_HPP

# include <stack>
# include <string>
# include <stdexcept>

// Custom exception classes for better error handling
class RPNException : public std::exception {
	private:
		std::string _message;
	public:
		RPNException(const std::string& message) : _message(message) {}
		virtual ~RPNException() throw() {}
		virtual const char* what() const throw() { return _message.c_str(); }
};

class RPN {
	private:
		std::stack<int> _stack;  // Stack to hold operands during calculation

		// Helper method to check if a character is an operator
		bool isOperator(char c) const;
		
		// Helper method to perform operation based on operator
		void performOperation(char op);

	public:
		// Constructors and destructor
		RPN();
		~RPN();

		// Calculates the result of an RPN expression
		int calculate(const std::string& expression);
};

#endif
