#include "textTreeNodeBlockWithBlocks.h"

textTreeNodeBlockWithBlocks::textTreeNodeBlockWithBlocks(
        int containsBlocksNum, textTreeNode *parent, int start, int end
        ) :
    textTreeNode(parent, start, end),
    containsBlocksNum(containsBlocksNum)
{
    //QString pattern =
}

textTreeNodeBlockWithBlocks::~textTreeNodeBlockWithBlocks()
{

}

bool textTreeNodeBlockWithBlocks::isClosingCondition()
{
    if (containsBlocksNum == 0) {
        _blockClosed = true;
        //setEnd(_start + _startToken.length() - 1);
        setEndToken("", _start + _startToken.length());
        return true;
    }

    bool ret = false;

    if (children.count() == containsBlocksNum) {
        if (children.at(containsBlocksNum - 1)->blockClosed()) {
            _blockClosed = true;
            //setEnd(children.at(containsBlocksNum - 1)->end() + children.at(containsBlocksNum - 1)->endToken().length() - 1);
            setEndToken(""
                        ,children.at(containsBlocksNum - 1)->end()
                        + children.at(containsBlocksNum - 1)->endToken().length());
            ret = true;
        }
    }

    return ret;
}
