#include "textTree.h"
#include <iostream>

textTree::textTree()
{
    initialize(0);
}

textTree::~textTree()
{
    delete _root;
    _root = 0;
}

void textTree::initialize(int textLength)
{
    _root = new textTreeNode(0, 0, textLength-1);
    currentNode = _root;
}

void textTree::clear(int textLength)
{
    delete _root;
    _root = 0;
    initialize(textLength);
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

textTreeNode* textTree::up()
{
    if (currentNode->parent() != 0) {
        currentNode = currentNode->parent();
    }
    return currentNode;
}
