#ifndef TEXTTREE_H
#define TEXTTREE_H

#include "textTreeNode.h"
#include <QRegExp>

class textTree {

public:
    textTree();
    ~textTree();
    void clear(int textLength);
    void setToken(const QString& token, int pos);
    textTreeNode* root() const;
    textTreeNode* down();
    textTreeNode* down(textTreeNode* newNode);
    textTreeNode* up();

private:
    enum blockType {blockUndef = 0, blockWithBlackets, blockWithLeftRight, blockWithZeroBlock, blockWithOneBlock, blockWithTwoBlocks};

private:
    void initialize(int textLength);
    blockType blockHeaderOf(const QString& token);

private:
    QRegExp regexpStartTokenBlacket;
    QRegExp regexpStartTokenLeftRight;
    QRegExp regexpStartTokenZeroBlock;
    QRegExp regexpStartTokenOneBlock;
    QRegExp regexpStartTokenTwoBlocks;

private:
    class textTreeNode* _root;
    class textTreeNode* currentNode;
};

#endif // TEXTTREE_H
