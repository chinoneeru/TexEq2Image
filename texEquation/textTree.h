#ifndef TEXTTREE_H
#define TEXTTREE_H

#include "textTreeNode.h"

class textTree {

public:
    textTree();
    ~textTree();
    void clear(int textLength);
    textTreeNode* root() const;
    textTreeNode* down();
    textTreeNode* up();

private:
    void initialize(int textLength);

private:
    class textTreeNode* _root;
    class textTreeNode* currentNode;
};

#endif // TEXTTREE_H
