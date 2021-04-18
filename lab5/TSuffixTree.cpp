#include "TSuffixTree.hpp"

TSuffixTree::TSuffixTree(std::string &str) {
    // adding a sentinel
    Text = str + '$';

    for (int i = 0; i < Text.length(); ++i) {
        Extend(i);
    }
}

void TSuffixTree::Extend(int phase) {
    TNode* LastCreatedInternalNode = nullptr; // for creating suffix links

    ++Remaining; // at every phase we should add one more suffix
    ++EndPos; // incrementing global end pos

    while (Remaining > 0) {
        if (ActiveLength == 0) {
            // searching for current character from active edge
            ActiveEdge = phase;
        }

        if (!ActiveNode->Edges[Text[ActiveEdge]]) {
            // there is no edge starting with current char -> creating it
            ActiveNode->Edges[Text[ActiveEdge]] = new TNode(phase, -1); // -1 is global end

            // updating LexMin edge for active node
            if ((Text[phase] < ActiveNode->LexMinEdge && Text[phase] != '$') || ActiveNode->LexMinEdge == 0) {
                ActiveNode->LexMinEdge = Text[phase];
            }

            // as we've created an internal node, we should update a suffix link
            if (LastCreatedInternalNode != nullptr) {
                LastCreatedInternalNode->SuffixLink = ActiveNode;
            }
            LastCreatedInternalNode = ActiveNode;
        } else {
            // there is an edge starting with current char -> walking along it
            TNode* NextNode = ActiveNode->Edges[Text[ActiveEdge]];
            int CurEnd = NextNode->End == -1 ? EndPos : NextNode->End;
            int EdgeLength = CurEnd - NextNode->Start + 1;

            if (ActiveLength >= EdgeLength) {
                // we should change active node
                ActiveEdge += EdgeLength;
                ActiveLength -= EdgeLength;
                ActiveNode = NextNode;
                continue;
            }

            if (Text[NextNode->Start + ActiveLength] == Text[phase]) {
                // there is already a suffix in the tree -> updating and stopping
                ++ActiveLength;
                // we could have created an internal node earlier -> updating suffix links
                if (LastCreatedInternalNode != nullptr) {
                    LastCreatedInternalNode->SuffixLink = ActiveNode;
                }
                break;
            }

            // there are no suffix in the tree -> creating new internal node
            TNode* InternalNode =  new TNode(NextNode->Start, NextNode->Start + ActiveLength - 1);
            TNode* LeafNode = new TNode(phase, -1);

            ActiveNode->Edges[Text[ActiveEdge]] = InternalNode;
            NextNode->Start += ActiveLength;
            InternalNode->Edges[Text[phase]] = LeafNode;
            InternalNode->Edges[Text[NextNode->Start]] = NextNode;

            // finding a LexMin edge for new internal node
            if (Text[NextNode->Start] < Text[LeafNode->Start]) {
                if (Text[NextNode->Start != '$']) {
                    InternalNode->LexMinEdge = Text[NextNode->Start];
                } else {
                    InternalNode->LexMinEdge = Text[LeafNode->Start];
                }
            } else {
                if (Text[LeafNode->Start] != '$') {
                    InternalNode->LexMinEdge = Text[LeafNode->Start];
                } else {
                    InternalNode->LexMinEdge = Text[NextNode->Start];
                }
            }

            // as we've created an internal node, we should add a suffix link
            if (LastCreatedInternalNode != nullptr) {
                LastCreatedInternalNode->SuffixLink = InternalNode;
            }
            LastCreatedInternalNode = InternalNode;
        }
        --Remaining; // we've added 1 suffix

        if (ActiveNode != Root) {
            ActiveNode = ActiveNode->SuffixLink;
        } else if (ActiveNode == Root && ActiveLength > 0) {
            --ActiveLength;
            ++ActiveEdge;
        }
    }
}

std::string TSuffixTree::FindLexMinString() {
    std::string LexMinString;
    TNode* CurNode = Root;

    while (LexMinString.length() < (Text.length() - 1) / 2) {
        CurNode = CurNode->Edges[CurNode->LexMinEdge];
        int CurEnd = CurNode->End == -1 ? EndPos : CurNode->End;

        for (int i = CurNode->Start; i <= CurEnd; ++i) {
            LexMinString += Text[i];
            if (LexMinString.length() == (Text.length() - 1) / 2) {
                break;
            }
        }
    }
    return LexMinString;
}

TSuffixTree::~TSuffixTree() {
    Delete(Root);
}

void TSuffixTree::Delete(TNode *node) {
    for (auto child : node->Edges) {
        Delete(child.second);
    }

    delete node;
}
