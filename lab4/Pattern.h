#ifndef DA_LAB4_PATTERN_H
#define DA_LAB4_PATTERN_H

#include <algorithm>
#include <vector>
#include <map>

class TPattern {
private:
    std::vector<unsigned int> Pattern;
    std::map<unsigned int, unsigned int> BCR;
    std::vector<int> N;
    std::vector<int> L;
    std::vector<int> l;

    void ReadPattern() {
        char c = ' ';
        std::string curNum;

        while (true) {
            c = getchar();
            if (c == ' ') {
                if (!curNum.empty()) {
                    Pattern.push_back(std::stoul(curNum));
                    curNum.clear();
                }
            }
            else if (c == '\n' || c == EOF) {
                if (!curNum.empty()) {
                    Pattern.push_back(std::stoul(curNum));
                }
                break;
            }
            else if (c >= '0' && c <= '9') {
                curNum.push_back(c);
            }
        }
    }

    // mapping each 'letter' in pattern to its rightmost position
    void BadCharRule() {
        for (int i = 0; i < Pattern.size(); ++i) {
            BCR[Pattern[i]] = i;
        }
    }

    std::vector<int> ZFunction(std::vector<unsigned int>& pattern) {
        int n = pattern.size();
        std::vector<int>Z(n);
        int left = 0, right = 0; // rightmost Z-block

        for (int i = 1; i < n; ++i) {
            if (i <= right) {
                // cur pos is inside rightmost Z-block
                Z[i] = std::min(right - i + 1, Z[i - left]);
            }

            // trivial algorithm
            while (i + Z[i] < n && pattern[Z[i]] == pattern[i + Z[i]]) {
                ++Z[i];
            }

            if (i + Z[i] - 1 > right) {
                // we should update coords of rightmost Z-block
                left = i;
                right = i + Z[i] - 1;
            }
        }
        return Z;
    }

    void NFunction() {
        std::vector<unsigned int> reversed_pattern = Pattern;
        std::reverse(reversed_pattern.begin(), reversed_pattern.end());
        auto Z = ZFunction(reversed_pattern);

        int n = Pattern.size();
        N.resize(n);
        for (int i = 0; i < n; ++i) {
            N[i] = Z[n - i - 1];
        }
    }

    // L[i] is the position of the rightmost element j: N[j] = sizeof(suffix) = n - i + 1
    void LFunction() {
        int n = N.size();
        L.resize(n);
        for (int i = 0; i < n - 1; ++i) {
            int j = n - N[i];
            if (j < n) {
                L[j] = i;
            }
        }
    }

    void lfunction() {
        int n = N.size();
        l.resize(n + 1);
        for (int i = n - 1; i >= 0; --i) {
            int j = n - i;
            if (N[j - 1] == j) {
                l[i] = j;
            }
            else {
                l[i] = l[i + 1];
            }
        }
    }

    long long GetBCRMove(unsigned int symbol, long long pos) {
        if (BCR.find(symbol) == BCR.end()) {
            return pos;
        }
        if (pos - BCR[symbol] > 0) {
            return pos - BCR[symbol];
        }
        return 1;

    }

    long long GetGSRMove(long long pos) {
        if (pos == Size() - 1) {
            return 1;
        }
        if (L[pos + 1] != 0) {
            return Size() - L[pos + 1] - 1;
        }
        return Size() - l[pos + 1];
    }

public:

    void Initialize() {
        ReadPattern();
        BadCharRule();
        NFunction();
        LFunction();
        lfunction();
    }

    unsigned int& operator[](long long id) {
        return Pattern[id];
    }

    long long Size() const {
        return Pattern.size();
    }

    long long GetMove(unsigned int symbol, long long pos) {
        return std::max(GetBCRMove(symbol, pos), std::max(GetGSRMove(pos), 1ll));
    }

    long long GetFinalMove() {
        return Size() - l[1];
    }
};


#endif //DA_LAB4_PATTERN_H
