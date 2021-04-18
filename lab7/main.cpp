#include <iostream>

// counts the amount of suitable numbers between left bound and right bound
long long Count(long long leftBound, long long rightBound, int m) {
    long long firstNum, lastNum; // first and last suitable numbers
    if (leftBound % m != 0) {
        firstNum = leftBound + m - leftBound % m;
    }
    else {
        firstNum = leftBound;
    }
    lastNum = rightBound - rightBound % m;

    if (firstNum <= lastNum) {
        // there are some suitable numbers in the interval
        return (lastNum - firstNum) / m + 1;
    }
    return 0;
}

int main() {
    std::string n;
    int m;
    std::cin >> n >> m;

    long long answer = 0;

    long long leftBound = 1, rightBound = 0; // current interval
    for (int i = 0; i < n.length(); ++i) {
        // updating bounds of interval
        if (i != 0) {
            leftBound *= 10;
            rightBound *= 10;
        }
        rightBound += n[i] - '0';

        answer += Count(leftBound, rightBound, m);
    }

    if (rightBound % m == 0) {
        // we should not include the initial number
        --answer;
    }
    std::cout << answer << "\n";
}
