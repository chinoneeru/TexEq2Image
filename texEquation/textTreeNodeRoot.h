#ifndef TEXTTREENODEROOT_H
#define TEXTTREENODEROOT_H

#include "textTreeNode.h"

class textTreeNodeRoot : public textTreeNode {

public:
    textTreeNodeRoot(int start = 0, int end = 0);
    ~textTreeNodeRoot();

public:
    bool isClosingCondition();
};

#endif // TEXTTREENODEROOT_H
