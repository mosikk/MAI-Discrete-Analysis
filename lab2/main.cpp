#include <iostream>
#include "TBTree.h"

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    TBTree tree;
    char cmd[257];
    while (std::cin >> cmd) {
        if (strcmp(cmd, "+") == 0) {
            TPair KV;
            std::cin >> KV.key >> KV.value;
            CheckString(KV.key);
            tree.Insert(KV);
        }
        else if (strcmp(cmd, "-") == 0) {
            std::cin >> cmd;
            CheckString(cmd);
            tree.Remove(cmd);
        }
        else if (strcmp(cmd, "!") == 0) {
            std::cin >> cmd;
            if (strcmp(cmd, "Save") == 0) {
                std::cin >> cmd;
                FILE* file = fopen(cmd, "wb");
                if (!file) {
                    std::cout << "ERROR: can't open a file\n";
                }
                else {
                    tree.Serialize(file);
                    fclose(file);
                }
            }
            else if (strcmp(cmd, "Load") == 0) {
                std::cin >> cmd;
                FILE* file = fopen(cmd, "rb");
                if (!file) {
                    std::cout << "ERROR: can't open a file\n";
                }
                else {
                    tree.Deserialize(file);
                    fclose(file);
                }
            }
        }
        else {
            CheckString(cmd);
            tree.Search(cmd);
        }
    }
}
