#include "textTree.h"
#include "textTreeNodeRoot.h"
#include "textTreeNodeBlockWithBlackets.h"
#include "textTreeNodeBlockWithLeftRight.h"
#include "textTreeNodeBlockWithBlocks.h"
#include <iostream>

textTree::textTree()
{
    QString pattern = "\\{";
    regexpStartTokenBlacket.setPattern(pattern);

    pattern = QString("%1|%2|%3|%4|%5")
            .arg("\\\\left\\s*\\(")
            .arg("\\\\left\\s*\\[")
            .arg("\\\\left\\s*\\.")
            .arg("\\\\left\\s*\\|")
            .arg("\\\\left\\s*\\\\\\{");
    regexpStartTokenLeftRight.setPattern(pattern);

    pattern = QString("%1|%2")
            .arg("[a-zA-Z0-9]")
            .arg("\\\\[a-zA-Z]+");
    regexpStartTokenZeroBlock.setPattern(pattern);

    pattern = QString("%1|%2")
            .arg("\\\\textrm")
            .arg("\\\\sqrt");
    regexpStartTokenOneBlock.setPattern(pattern);

    pattern = "\\\\frac";
    regexpStartTokenTwoBlocks.setPattern(pattern);

    initialize(0);
}

textTree::~textTree()
{
    delete _root;
    _root = 0;
}

void textTree::initialize(int textLength)
{
    _root = new textTreeNodeRoot(0, textLength - 1);
    currentNode = _root;
}

void textTree::clear(int textLength)
{
    delete _root;
    _root = 0;
    initialize(textLength);
}

void textTree::setToken(const QString &token, int pos)
{
    textTreeNode* newNode = 0;
    blockType type = blockHeaderOf(token);

    if (type != blockUndef) {

        switch (type) {
        case blockWithBlackets:
            newNode = new textTreeNodeBlockWithBlackets(currentNode);
            break;
        case blockWithLeftRight:
            newNode = new textTreeNodeBlockWithLeftRight(currentNode);
            break;
        case blockWithZeroBlock:
            newNode = new textTreeNodeBlockWithBlocks(0, currentNode);
            break;
        case blockWithOneBlock:
            newNode = new textTreeNodeBlockWithBlocks(1, currentNode);
            break;
        case blockWithTwoBlocks:
            newNode = new textTreeNodeBlockWithBlocks(2, currentNode);
            break;
        default:
            break;
        }

        down(newNode);
        currentNode->setStartToken(token, pos);

        while (currentNode->isClosingCondition()) {

            if (currentNode->parent() == 0) break;
            up();

        }

    } else {

        bool first = true;
        currentNode->setTokenToBeChecked(token);

        while (currentNode->isClosingCondition()) {

            if (first) {
                currentNode->setEndToken(token, pos);
                first = false;
            }

            if (currentNode->parent() == 0) break;
            up();

        }

    }
}

textTree::blockType textTree::blockHeaderOf(const QString &token)
{
    if (regexpStartTokenLeftRight.exactMatch(token))
        return blockWithLeftRight;

    if (regexpStartTokenBlacket.exactMatch(token))
        return blockWithBlackets;

    if (regexpStartTokenTwoBlocks.exactMatch(token))
        return blockWithTwoBlocks;

    if (regexpStartTokenOneBlock.exactMatch(token))
        return blockWithOneBlock;

    if(regexpStartTokenZeroBlock.exactMatch(token))
        return blockWithZeroBlock;

    return blockUndef;
}

textTreeNode* textTree::root() const
{
    return _root;
}

textTreeNode* textTree::down()
{
    currentNode = currentNode->addChilde();
    return currentNode;
}

textTreeNode* textTree::down(textTreeNode *newNode)
{
    currentNode = currentNode->addChilde(newNode);
    return currentNode;
}

textTreeNode* textTree::up()
{
    if (currentNode->parent() != 0) {
        currentNode = currentNode->parent();
    }
    return currentNode;
}
