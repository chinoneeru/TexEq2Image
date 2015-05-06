#ifndef TEXTTREENODEBLOCKWITHBLACKETS_H
#define TEXTTREENODEBLOCKWITHBLACKETS_H

#include "textTreeNode.h"

class textTreeNodeBlockWithBlackets : public textTreeNode {

public:
    textTreeNodeBlockWithBlackets(class textTreeNode* parent = 0, int start = 0, int end = 0);
    ~textTreeNodeBlockWithBlackets();

public:
    bool isClosingCondition();

};

#endif // TEXTTREENODEBLOCKWITHBLACKETS_H
