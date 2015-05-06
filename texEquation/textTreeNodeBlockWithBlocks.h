#ifndef TEXTTREENODEBLOCKWITHBLOCKS_H
#define TEXTTREENODEBLOCKWITHBLOCKS_H

#include "textTreeNode.h"

class textTreeNodeBlockWithBlocks : public textTreeNode {

public:
    textTreeNodeBlockWithBlocks(int containsBlocksNum, class textTreeNode* parent = 0, int start = 0, int end = 0);
    ~textTreeNodeBlockWithBlocks();

public:
    bool isClosingCondition();

private:
    int containsBlocksNum;
    //QRegExp closeTokenExpression;

};

#endif // TEXTTREENODEBLOCKWITHBLOCKS_H
