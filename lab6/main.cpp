#include "TBigInt.hpp"

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    TBigInt zero(0);
    TBigInt one(1);

    std::string left, right;
    char op;

    while (std::cin >> left >> right >> op) {
        TBigInt leftNum(left);
        TBigInt rightNum(right);

        if (op == '+') {
            std::cout << leftNum + rightNum << "\n";

        } else if (op == '-') {
            if (leftNum < rightNum) {
                std::cout << "Error\n";
            } else {
                std::cout << leftNum - rightNum << "\n";
            }

        } else if (op == '*') {
            std::cout << leftNum * rightNum << "\n";

        } else if (op == '/') {
            if (rightNum == zero) {
                std::cout << "Error\n";
            } else if (leftNum < rightNum) {
                std::cout << "0\n";
            } else {
                std::cout << leftNum / rightNum << "\n";
            }

        } else if (op == '^') {
            if (leftNum == zero && rightNum == zero) {
                std::cout << "Error\n";
            } else {
                std::cout << leftNum.Pow(rightNum) << "\n";
            }

        } else if (op == '<') {
            std::cout << (leftNum < rightNum ? "true" : "false") << "\n";

        } else if (op == '>') {
            std::cout << (leftNum > rightNum ? "true" : "false") << "\n";

        } else if (op == '=') {
            std::cout << (leftNum == rightNum ? "true" : "false") << "\n";

        } else {
            std::cout << "Error\n";
        }
    }
}