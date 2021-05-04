#include <iostream>
#include <vector>

long long getNextPos(short num, long long curPos, std::vector<short> &nums, bool reverseOrder = false) {
    if (nums.empty() || curPos < 0 || curPos >= nums.size()) {
        return 0;
    }
    if (!reverseOrder) {
        while (nums[curPos] != num && curPos + 1 < nums.size()) {
            ++curPos;
        }
    } else {
        while (nums[curPos] != num && curPos - 1 >= 0) {
            --curPos;
        }
    }
    return curPos;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    const int MAX_NUM = 3;

    long long n;
    std::cin >> n;
    std::vector<short> nums(n);
    std::vector<long long> counts(MAX_NUM);
    for (long long i = 0; i < n; ++i) {
        std::cin >> nums[i];
        ++counts[nums[i] - 1];
    }

    long long swaps = 0;

    // posi_j - the position of number j in a zone i

    // | zone1 | zone2 | zone3 |
    // | ------|-------|-------|
    // | 11111 | 22222 | 33333 | - sorted array

    long long pos2_1 = getNextPos(1, counts[0], nums);
    long long pos3_1 = getNextPos(1, n - 1, nums, true);
    long long pos3_2 = getNextPos(2, counts[0] + counts[1], nums);

    for (int i = 0; i < n; ++i) {
        if (i < counts[0] && nums[i] != 1) {
            // found number 2 or 3 which must be replaced by 1
            if (nums[i] == 2) {
                std::swap(nums[i], nums[pos2_1]);
                ++swaps;
                pos2_1 = getNextPos(1, pos2_1, nums);
            } else {
                std::swap(nums[i], nums[pos3_1]);
                ++swaps;
                pos3_1 = getNextPos(1, pos3_1, nums, true);
            }
        } else if (counts[0] <= i && i < counts[0] + counts[1] && nums[i] != 2) {
            // found number 3 which must be replaced by 2
            // numbers 1 can't be here, they are already placed correctly
            std::swap(nums[i], nums[pos3_2]);
            ++swaps;
            pos3_2 = getNextPos(2, pos3_2, nums);
        }
    }

    std::cout << swaps << "\n";
}
