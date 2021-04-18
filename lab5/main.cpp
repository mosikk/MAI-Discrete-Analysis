#include <iostream>

#include "TSuffixTree.hpp"

int main() {
    std::string str;
    std::cin >> str;

    // double the string
    str += str;
    TSuffixTree tree(str);
    std::cout << tree.FindLexMinString() << "\n";
}
