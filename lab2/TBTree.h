#ifndef DA_LAB2_V2_TBTREE_H
#define DA_LAB2_V2_TBTREE_H

#include "TNode.h"

void SearchNode(TNode *node, char* str, TNode *&res, int &pos);
void SplitChild(TNode *parent, int pos); // splitting parent at pos
void InsertNode(TNode *node, TPair &KV);
int SearchInNode(TNode *node, char* str); // searching str. pos is index of str or index of a child with str
void RemoveNode(TNode *node, char* str);
void RemoveFromNode(TNode *node, int pos); // remove key[pos] in current node by making shifts
void MergeNodes(TNode *parent, int pos); // merging left child, parent[pos] and right child to left child
void Rebalance(TNode *node, int &pos); // rebalancing tree if a node has critical size
bool NodeToFile(TNode *node, FILE *file);
bool FileToTree(TNode *node, FILE *file);
void DeleteTree(TNode *node);


class TBTree {
public:
    TNode *root;

    TBTree() {
        root = new TNode;
    }

    ~TBTree() {
        Delete();
    }

    void Search(char *str) const {
        TNode *res;
        int pos;
        SearchNode(root, str, res, pos);
        if (res == nullptr) {
            std::cout << "NoSuchWord\n";
        } else {
            std::cout << "OK: " << res->keys[pos].value << "\n";
        }
    }

    void Insert(TPair &KV) {
        TNode *search_res;
        int search_pos;
        SearchNode(root, KV.key, search_res, search_pos);
        if (search_res != nullptr) {
            std::cout << "Exist\n";
            return;
        }
        // if a root is full, we'll make a split
        if (root->keys_num == 2 * DEGREE - 1) {
            TNode* new_root = new TNode;
            new_root->is_leaf = false;
            new_root->children[0] = root;
            root = new_root;
            SplitChild(root, 0);
        }
        InsertNode(root, KV);
        std::cout << "OK\n";
    }

    void Remove(char *str) {
        TNode *res;
        int pos;
        SearchNode(root, str, res, pos);
        if (res == nullptr) {
            std::cout << "NoSuchWord\n";
        } else {
            RemoveNode(root, str);
            std::cout << "OK\n";
            // if our root is empty, its child becomes a new one
            if (root->keys_num == 0 && !root->is_leaf) {
                TNode* tmp = root->children[0];
                delete root;
                root = tmp;
            }
        }
    }

    void Serialize(FILE* file) {
        if (NodeToFile(root, file)) {
            std::cout << "OK\n";
        }
    }

    void Deserialize(FILE* file) {
        if (FileToTree(root, file)) {
            std::cout << "OK\n";
        }
    }

private:
    void Delete() {
        DeleteTree(root);
    }
};

void SearchNode(TNode *node, char* str, TNode *&res, int &pos) {
    int i = 0;
    // searching for key in array
    while (i < node->keys_num && node->keys[i] < str) {
        ++i;
    }

    if (i < node->keys_num && node->keys[i] == str) {
        // found a key
        res = node;
        pos = i;
        return;
    }

    // no key and no children -> we don't have this element
    if (node->is_leaf) {
        res = nullptr;
        return;
    }

    // no key but we have children -> recursively searching in child's array
    SearchNode(node->children[i], str, res, pos);
}

// splitting parent at pos
void SplitChild(TNode *parent, int pos) {
    TNode *child = parent->children[pos];

    // creating new node
    TNode* new_child = new TNode;
    new_child->is_leaf = child->is_leaf;
    new_child->keys_num = DEGREE - 1;

    // copying half of keys to new child
    for (int i = 0; i < DEGREE - 1; ++i) {
        new_child->keys[i] = child->keys[i + DEGREE];
    }

    // copying half of child pointers to new child if children are not leaves
    if (!child->is_leaf) {
        for (int i = 0; i < DEGREE; ++i) {
            new_child->children[i] = child->children[i + DEGREE];
            child->children[i + DEGREE] = nullptr;
        }
    }

    child->keys_num = DEGREE - 1;

    // inserting a new child pointer to parent's children
    for (int i = parent->keys_num; i >= pos + 1; --i) {
        parent->children[i + 1] = parent->children[i];
        parent->children[i] = nullptr;
    }
    if (parent->children[pos + 1] != nullptr) {
        DeleteTree(parent->children[pos + 1]);
    }
    parent->children[pos + 1] = new_child;

    // inserting middle element to parent's keys
    for (int i = parent->keys_num - 1; i >= pos; --i) {
        parent->keys[i + 1] = parent->keys[i];
    }
    parent->keys[pos] = child->keys[DEGREE - 1];
    ++parent->keys_num;
}

void InsertNode(TNode *node, TPair &KV) {
    int i = node->keys_num - 1;

    if (node->is_leaf) { // making a shift and entering a new element
        while (i >= 0 && KV < node->keys[i]) {
            node->keys[i + 1] = node->keys[i];
            --i;
        }
        node->keys[i + 1] = KV;
        ++node->keys_num;
    }
    else { // searching for a child which we'll place a new element in
        while (i >= 0 && KV < node->keys[i]) {
            --i;
        }
        ++i;
        if (node->children[i]->keys_num == 2 * DEGREE - 1) {
            SplitChild(node, i);
            // after splitting we should choose where to place a new element
            if (node->keys[i] < KV) {
                ++i;
            }
        }
        InsertNode(node->children[i], KV);
    }
}

// searching str in node. pos is index of str or index of a child with str
int SearchInNode(TNode* node, char* str) {
    int left = -1;
    int right = node->keys_num;
    while (right - left > 1) {
        int mid = (left + right) / 2;
        if (node->keys[mid] < str) {
            left = mid;
        }
        else {
            right = mid;
        }
    }

    if (right < node->keys_num && node->keys[right] == str) { // found a key
        return right;
    }

    // -i - 1 means that our str is in child[i]
    return -right - 1;
}

void RemoveNode(TNode *node, char* str) {

    // checking if our str is in current node
    int pos = SearchInNode(node, str);

    // str is in current leaf node
    if (pos >= 0 && node->is_leaf) {
        RemoveFromNode(node, pos);
        return;
    }

    // str is in current NONleaf node
    if (pos >= 0 && !node->is_leaf) {
        TNode *left_child = node->children[pos];

        // left child is big enough -> looking for the largest key in left child
        if (left_child->keys_num >= DEGREE) {
            while (!left_child->is_leaf) {
                left_child = left_child->children[left_child->keys_num];
            }

            // placing largest left key instead of one we want to remove
            node->keys[pos] = left_child->keys[left_child->keys_num - 1];

            // removing largest left key in leaf node
            char* new_str = new char[strlen(left_child->keys[left_child->keys_num - 1].key) + 1];
            strcpy(new_str, left_child->keys[left_child->keys_num - 1].key);
            RemoveNode(node->children[pos], new_str);
            delete[] new_str;
            return;
        }

        TNode *right_child = node->children[pos + 1];

        // right child is big enough -> looking for the smallest key in right child
        if (right_child->keys_num >= DEGREE) {
            while (!right_child->is_leaf) {
                right_child = right_child->children[0];
            }

            // placing smallest right key instead of one we want to remove
            node->keys[pos] = right_child->keys[0];

            // removing smallest right key in leaf node
            char* new_str = new char[strlen(right_child->keys[0].key) + 1];
            strcpy(new_str, right_child->keys[0].key);
            RemoveNode(node->children[pos + 1], new_str);
            delete[] new_str;
            return;
        }

        // both children have DEGREE - 1 keys -> merging them
        MergeNodes(node, pos);
        RemoveNode(node->children[pos], str);
        return;
    }

    pos = -pos - 1;
    // no key in current node
    if (node->children[pos]->keys_num == DEGREE - 1) {
        Rebalance(node, pos);
    }
    RemoveNode(node->children[pos], str);
}

// remove key[pos] in current node by making shifts
void RemoveFromNode(TNode *node, int pos) {
    for (int i = pos; i < node->keys_num - 1; ++i) {
        node->keys[i] = node->keys[i + 1];
    }
    if (!node->is_leaf) {
        for (int i = pos; i < node->keys_num; ++i) {
            node->children[i] = node->children[i + 1];
            node->children[i + 1] = nullptr;
        }
    }
    --node->keys_num;
}

// merging left child, parent[pos] and right child to left child
void MergeNodes(TNode *parent, int pos) {
    TNode *left_child = parent->children[pos];
    TNode *right_child = parent->children[pos + 1];

    // copying parent[pos] and right child keys to left child
    left_child->keys[left_child->keys_num++] = parent->keys[pos];
    for (int i = 0; i < right_child->keys_num; ++i) {
        left_child->keys[left_child->keys_num++] = right_child->keys[i];
    }

    // copying children of right child is necessary
    if (!right_child->is_leaf) {
        for (int i = 0; i <= right_child->keys_num; ++i) {
            left_child->children[DEGREE + i] = right_child->children[i];
            right_child->children[i] = nullptr;
        }
    }

    right_child->keys_num = 0;
    delete right_child;
    RemoveFromNode(parent, pos);
    parent->children[pos] = left_child;
}

// rebalancing tree if a node has critical size
void Rebalance(TNode *node, int& pos) {
    TNode *child = node->children[pos];

    if (pos - 1 >= 0 && node->children[pos - 1]->keys_num >= DEGREE) {
        // left neighbour is big enough
        TNode *left_neighbour = node->children[pos - 1];

        // shifting child to the right
        for (int i = child->keys_num - 1; i >= 0; --i) {
            child->keys[i + 1] = child->keys[i];
        }

        if (!child->is_leaf) {
            for (int i = child->keys_num; i >= 0; --i) {
                child->children[i + 1] = child->children[i];
                child->children[i] = nullptr;
            }
        }

        // a key from parent goes to child
        child->keys[0] = node->keys[pos - 1];
        child->children[0] = left_neighbour->children[left_neighbour->keys_num];
        left_neighbour->children[left_neighbour->keys_num] = nullptr;
        ++child->keys_num;

        // last key of left neighbour goes to parent
        node->keys[pos - 1] = left_neighbour->keys[left_neighbour->keys_num - 1];

        --left_neighbour->keys_num;

    } else if (pos + 1 <= node->keys_num && node->children[pos + 1]->keys_num >= DEGREE) {
        // right neighbour is big enough
        TNode *right_neighbour = node->children[pos + 1];

        // a key from parent goes to child
        child->keys[child->keys_num] = node->keys[pos];
        child->children[child->keys_num + 1] = right_neighbour->children[0];
        right_neighbour->children[0] = nullptr;
        ++child->keys_num;

        // first key of right neighbour goes to parent
        node->keys[pos] = right_neighbour->keys[0];
        RemoveFromNode(right_neighbour, 0);

    } else if (pos - 1 >= 0 && node->children[pos - 1]->keys_num == DEGREE - 1) {
        // we have a left neighbour with a critical size -> merging with him
        MergeNodes(node, pos - 1);
        --pos;
    }
    else if (pos + 1 <= node->keys_num && node->children[pos + 1]->keys_num == DEGREE - 1) {
        // we have a right neighbour with a critical size -> merging with him
        MergeNodes(node, pos);
    }
}

bool NodeToFile(TNode *node, FILE *file) {
    bool status = true;
    if (fwrite(&node->is_leaf, sizeof(bool), 1, file) == 0) {
        std::cout << "ERROR: can't write to file\n";
        return false;
    }
    if (fwrite(&node->keys_num, sizeof(int), 1, file) == 0) {
        std::cout << "ERROR: can't write to file\n";
        return false;
    }
    for (int i = 0; i < node->keys_num; ++i) {
        int len = strlen(node->keys[i].key);
        if (fwrite(&len, sizeof(int), 1, file) == 0) {
            std::cout << "ERROR: can't write to file\n";
            return false;
        }
        if (fwrite(node->keys[i].key, sizeof(char), len, file) == 0) {
            std::cout << "ERROR: can't write to file\n";
            return false;
        }
        if (fwrite(&node->keys[i].value, sizeof(unsigned long long), 1, file) == 0) {
            std::cout << "ERROR: can't write to file\n";
            return false;
        }
    }
    if (!node->is_leaf) {
        for (int i = 0; i < node->keys_num + 1; ++i) {
            status |= NodeToFile(node->children[i], file);
        }
    }
    return status;
}

bool FileToTree(TNode *node, FILE *file) {
    bool status = true;
    if (fread(&node->is_leaf, sizeof(bool), 1, file) == 0) {
        std::cout << "ERROR: can't read from file\n";
        return false;
    }
    if (fread(&node->keys_num, sizeof(int), 1, file) == 0) {
        std::cout << "ERROR: can't read from file\n";
        return false;
    }
    for (int i = 0; i < node->keys_num; ++i) {
        int len;
        if (fread(&len, sizeof(int), 1, file) == 0) {
            std::cout << "ERROR: can't read from file\n";
            return false;
        }
        if (fread(node->keys[i].key, sizeof(char), len, file) == 0) {
            std::cout << "ERROR: can't read from file\n";
            return false;
        }
        node->keys[i].key[len] = '\0';
        if (fread(&node->keys[i].value, sizeof(unsigned long long), 1, file) == 0) {
            std::cout << "ERROR: can't read from file\n";
            return false;
        }
    }
    if (node->is_leaf) {
        for (int i = 0; i < 2 * DEGREE + 1; ++i) {
            if (node->children[i] != nullptr) {
                DeleteTree(node->children[i]);
                node->children[i] = nullptr;
            }
        }
    }
    else {
        for (int i = 0; i < node->keys_num + 1; ++i) {
            if (node->children[i] == nullptr) {
                node->children[i] = new TNode;
            }
            status |= FileToTree(node->children[i], file);
        }
    }
    return status;
}

void DeleteTree(TNode *node) {
    if (node == nullptr) {
        return;
    }
    if (!node->is_leaf) {
        for (int i = 0; i < 2 * DEGREE + 1; ++i) {
            if (node->children[i] != nullptr) {
                DeleteTree(node->children[i]);
                node->children[i] = nullptr;
            }
        }
    }
    delete node;
}


#endif //DA_LAB2_V2_TBTREE_H