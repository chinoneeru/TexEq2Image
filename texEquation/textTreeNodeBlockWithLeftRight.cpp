#include "textTreeNodeBlockWithLeftRight.h"

textTreeNodeBlockWithLeftRight::textTreeNodeBlockWithLeftRight(
        textTreeNode *parent, int start, int end
        ) :
    textTreeNode(parent, start, end)
{
    QString pattern = QString("%1|%2|%3|%4|%5")
            .arg("\\\\right\\s*\\)")
            .arg("\\\\right\\s*\\]")
            .arg("\\\\right\\s*\\.")
            .arg("\\\\right\\s*\\|")
            .arg("\\\\right\\s*\\\\\\}");
    closeTokenExpression.setPattern(pattern);
}

textTreeNodeBlockWithLeftRight::~textTreeNodeBlockWithLeftRight()
{

}

bool textTreeNodeBlockWithLeftRight::isClosingCondition()
{
    if (closeTokenExpression.exactMatch(_tokenToBeChecked)) {
        return true;
    } else {
        return false;
    }
}
