/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPN.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschmitz <eschmitz@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 17:21:58 by eschmitz          #+#    #+#             */
/*   Updated: 2025/05/15 17:23:46 by eschmitz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RPN.hpp"
#include <cctype>
#include <cstdlib>
#include <iostream>

// Constructor
RPN::RPN() {
    // No initialization needed for the stack
}

// Destructor
RPN::~RPN() {
    // No cleanup needed
}

// Check if character is an operator
bool RPN::isOperator(char c) const {
    return (c == '+' || c == '-' || c == '*' || c == '/');
}

// Perform the specified operation on the top two elements of the stack
void RPN::performOperation(char op) {
    // Check if we have at least two operands on the stack
    if (_stack.size() < 2) {
        throw RPNException("Error: Not enough operands for operation");
    }

    // Get operands (note: stack has LIFO order, so second operand is popped first)
    int b = _stack.top();
    _stack.pop();
    int a = _stack.top();
    _stack.pop();

    // Perform operation based on the operator
    switch (op) {
        case '+':
            _stack.push(a + b);
            break;
        case '-':
            _stack.push(a - b);
            break;
        case '*':
            _stack.push(a * b);
            break;
        case '/':
            // Check for division by zero
            if (b == 0) {
                throw RPNException("Error: Division by zero");
            }
            _stack.push(a / b);
            break;
        default:
            throw RPNException("Error: Invalid operator");
    }
}

// Calculate the result of an RPN expression
int RPN::calculate(const std::string& expression) {
    // Clear the stack for a fresh calculation
    while (!_stack.empty()) {
        _stack.pop();
    }

    // Process each character in the expression
    for (size_t i = 0; i < expression.length(); i++) {
        char c = expression[i];

        // Skip whitespace
        if (std::isspace(c)) {
            continue;
        }

        // If it's a digit, push it onto the stack
        if (std::isdigit(c)) {
            _stack.push(c - '0');  // Convert char to integer
        }
        // If it's an operator, perform the operation
        else if (isOperator(c)) {
            performOperation(c);
        }
        // Invalid character
        else {
            throw RPNException("Error: Invalid token in expression");
        }
    }

    // After processing, there should be exactly one number on the stack (the result)
    if (_stack.size() != 1) {
        throw RPNException("Error: Invalid expression format");
    }

    // Return the final result
    return _stack.top();
}
