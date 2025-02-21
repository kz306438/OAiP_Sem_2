#include <iostream>
#include <stack>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <cctype>
#include <limits>

int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

bool isValidExpression(const std::string& expression) {
    int balance = 0;
    for (char ch : expression) {
        if (!(std::isalpha(ch) || std::isspace(ch) || ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '(' || ch == ')')) {
            std::cout << "Error: Invalid character '" << ch << "' in expression.\n";
            return false;
        }
        if (ch == '(') balance++;
        if (ch == ')') balance--;
        if (balance < 0) {
            std::cout << "Error: Mismatched parentheses.\n";
            return false;
        }
    }
    if (balance != 0) {
        std::cout << "Error: Mismatched parentheses.\n";
        return false;
    }
    return true;
}

std::string infixToRPN(const std::string& expression) {
    std::stack<char> operators;
    std::stringstream output;
    for (size_t i = 0; i < expression.size(); ++i) {
        char ch = expression[i];
        if (std::isspace(ch)) continue;
        if (std::isalpha(ch)) {
            output << ch << ' ';
        }
        else if (ch == '(') {
            operators.push(ch);
        }
        else if (ch == ')') {
            while (!operators.empty() && operators.top() != '(') {
                output << operators.top() << ' ';
                operators.pop();
            }
            operators.pop();
        }
        else {
            while (!operators.empty() && precedence(operators.top()) >= precedence(ch)) {
                output << operators.top() << ' ';
                operators.pop();
            }
            operators.push(ch);
        }
    }
    while (!operators.empty()) {
        output << operators.top() << ' ';
        operators.pop();
    }
    return output.str();
}

double evaluateRPN(const std::string& rpn, const std::unordered_map<char, double>& values) {
    std::stack<double> operands;
    std::stringstream tokens(rpn);
    std::string token;
    while (tokens >> token) {
        if (std::isalpha(token[0])) {
            operands.push(values.at(token[0]));
        }
        else {
            if (operands.size() < 2) {
                throw std::runtime_error("Error: Malformed expression.");
            }
            double b = operands.top(); operands.pop();
            double a = operands.top(); operands.pop();
            switch (token[0]) {
            case '+': operands.push(a + b); break;
            case '-': operands.push(a - b); break;
            case '*': operands.push(a * b); break;
            case '/':
                if (b == 0) throw std::runtime_error("Error: Division by zero.");
                operands.push(a / b);
                break;
            default: throw std::runtime_error("Error: Unknown operator.");
            }
        }
    }
    if (operands.size() != 1) {
        throw std::runtime_error("Error: Malformed expression.");
    }
    return operands.top();
}

double getValidatedInput(char var) {
    double value;
    while (true) {
        std::cout << "Enter value for " << var << ": ";
        if (std::cin >> value) return value;
        std::cout << "Invalid input. Please enter a number.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

int main() {
    std::string expression;
    std::cout << "Enter expression (use variables a, b, c, d, e): ";
    std::getline(std::cin, expression);

    if (!isValidExpression(expression)) {
        return 1;
    }

    std::string rpn = infixToRPN(expression);
    std::cout << "Reverse Polish Notation: " << rpn << std::endl;

    std::unordered_map<char, double> values;
    for (char var : {'a', 'b', 'c', 'd', 'e'}) {
        values[var] = getValidatedInput(var);
    }

    try {
        double result = evaluateRPN(rpn, values);
        std::cout << "Result: " << result << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
        return 1;
    }

    return 0;
}
