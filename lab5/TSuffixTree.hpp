#ifndef DA_LAB5_TSUFFIXTREE_H
#define DA_LAB5_TSUFFIXTREE_H

#include <unordered_map>
#include <iostream>

class TNode {
public:
    TNode(int left = 0, int right = 0) : Start(left), End(right) {}

    std::unordered_map<char, TNode*> Edges;
    TNode* SuffixLink = nullptr;

    int Start;
    int End;

    char LexMinEdge = 0; // edge with lexicographically minimal char
};

class TSuffixTree {
public:
    std::string Text;
    int EndPos = -1; // global variable for End position

    TNode* Root = new TNode();

    // a point where we will start the next phase
    TNode* ActiveNode = Root; // start node
    int ActiveEdge = -1; // id of the edge
    int ActiveLength = 0; // how far should we go along the edge

    int Remaining = 0; // amount of suffixed we should add


    TSuffixTree(std::string &str);
    ~TSuffixTree();

    void Extend(int phase);

    std::string FindLexMinString();

    void Delete (TNode* node);

};


#endif //DA_LAB5_TSUFFIXTREE_H
