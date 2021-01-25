#ifndef DA_LAB2_V2_TNODE_H
#define DA_LAB2_V2_TNODE_H

#include <cstring>

const int DEGREE = 5;

struct TPair {
    char key[257];
    unsigned long long value;
};

class TNode {
public:
    bool is_leaf = true;
    int keys_num = 0;
    TPair keys[2 * DEGREE];
    TNode* children[2 * DEGREE + 1];

    TNode() {
        for (int i = 0; i < 2 * DEGREE + 1; ++i) {
            children[i] = nullptr;
        }
    }
};

int Min(int lhs, int rhs) {
    if (lhs < rhs) {
        return lhs;
    }
    return rhs;
}

bool operator<(TPair &lhs, TPair &rhs) {
    int lhs_len = strlen(lhs.key);
    int rhs_len = strlen(rhs.key);
    for (int i = 0; i < Min(rhs_len, lhs_len); ++i) {
        if (lhs.key[i] != rhs.key[i]) {
            return lhs.key[i] < rhs.key[i];
        }
    }
    return lhs_len < rhs_len;
}

bool operator<(TPair &lhs, char rhs[]) {
    int lhs_len = strlen(lhs.key);
    int rhs_len = strlen(rhs);
    for (int i = 0; i < Min(rhs_len, lhs_len); ++i) {
        if (lhs.key[i] != rhs[i]) {
            return lhs.key[i] < rhs[i];
        }
    }
    return lhs_len < rhs_len;
}

bool operator==(TPair &lhs, char rhs[]) {
    int lhs_len = strlen(lhs.key);
    int rhs_len = strlen(rhs);
    if (lhs_len != rhs_len) {
        return false;
    }
    for (int i = 0; i < Min(rhs_len, lhs_len); ++i) {
        if (lhs.key[i] != rhs[i]) {
            return false;
        }
    }
    return true;
}

void CheckString(char *key) {
    int i = 0;
    while (key[i] != '\0') {
        if (key[i] >= 'A' && key[i] <= 'Z') {
            key[i] += 'a' - 'A';
        }
        ++i;
    }
}

#endif //DA_LAB2_V2_TNODE_H