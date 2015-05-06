#include "textTreeNodeRoot.h"

textTreeNodeRoot::textTreeNodeRoot(int start, int end):
    textTreeNode(0, start, end)
{

}

textTreeNodeRoot::~textTreeNodeRoot()
{

}

bool textTreeNodeRoot::isClosingCondition()
{
    return true;
}
