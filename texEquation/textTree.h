#ifndef TEXTTREE_H
#define TEXTTREE_H

#include "textTreeNode.h"

class textTree {

public:
    textTree();
    ~textTree();
    void clear();
    textTreeNode* root() const;
    textTreeNode* down();
    textTreeNode* up();

private:
    void initialize();

private:
    class textTreeNode* _root;
    class textTreeNode* currentNode;
};

#endif // TEXTTREE_H
