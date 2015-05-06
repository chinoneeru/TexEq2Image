#ifndef TEXTTREENODEBLOCKWITHLEFTRIGHT_H
#define TEXTTREENODEBLOCKWITHLEFTRIGHT_H

#include "textTreeNode.h"
#include <QRegExp>

class textTreeNodeBlockWithLeftRight : public textTreeNode {

public:
    textTreeNodeBlockWithLeftRight(class textTreeNode* parent = 0, int start = 0, int end = 0);
    ~textTreeNodeBlockWithLeftRight();

public:
    bool isClosingCondition();

private:
    QRegExp closeTokenExpression;

};

#endif // TEXTTREENODEBLOCKWITHLEFTRIGHT_H
