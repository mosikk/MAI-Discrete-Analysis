#include <iostream>
#include <vector>

#include "Pattern.h"

void ReadText(std::vector<unsigned int>& text, std::vector<std::pair<unsigned int, unsigned int>>& positions) {
    unsigned int curPos = 0;
    unsigned int curLine = 0;
    char c = ' ';
    std::string curNum;

    while (true) {
        c = getchar();
        if (c == ' ') {
            if (!curNum.empty()) {
                text.push_back(std::stoul(curNum));
                positions.emplace_back(curLine, curPos);
                curNum.clear();
                ++curPos;
            }
        }
        else if (c == '\n') {
            if (!curNum.empty()) {
                text.push_back(std::stoul(curNum));
                positions.emplace_back(curLine, curPos);
                curNum.clear();
                curPos = 0;
            }
            ++curLine;
        }
        else if (c == EOF) {
            if (!curNum.empty()) {
                text.push_back(std::stoul(curNum));
                positions.emplace_back(curLine, curPos);
            }
            break;
        }
        else if (c >= '0' && c <= '9') {
            curNum.push_back(c);
        }
    }
}


int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    TPattern pattern;
    pattern.Initialize();
    long long m = pattern.Size();

    std::vector<unsigned int> text; // part of a text with size = m
    std::vector<std::pair<unsigned int, unsigned int>> positions; // positions of text elements

    ReadText(text, positions);

    long long k = m - 1;
    while (k < text.size()) {
        long long i = k;
        long long j = m - 1;
        while (j >= 0 && pattern[j] == text[i]) {
            --i;
            --j;
        }
        if (j == -1) {
            std::cout << positions[k - m + 1].first + 1 << ", " << positions[k - m + 1].second + 1 << "\n";
            long long move = pattern.GetFinalMove();
            k += move;
        }
        else {
            long long move = pattern.GetMove(text[i], j);
            k += move;
        }
    }
}