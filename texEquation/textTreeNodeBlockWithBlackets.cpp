#include "textTreeNodeBlockWithBlackets.h"

textTreeNodeBlockWithBlackets::textTreeNodeBlockWithBlackets(
        textTreeNode *parent, int start, int end
        ) :
    textTreeNode(parent, start, end)
{

}

textTreeNodeBlockWithBlackets::~textTreeNodeBlockWithBlackets()
{

}

bool textTreeNodeBlockWithBlackets::isClosingCondition()
{
    if (_tokenToBeChecked == "}") {
        return true;
    } else {
        return false;
    }
}
