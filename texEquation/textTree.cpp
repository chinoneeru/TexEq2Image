#include "textTree.h"
#include <iostream>

textTree::textTree()
{
    initialize();
}

textTree::~textTree()
{
    delete _root;
    _root = 0;
}

void textTree::initialize()
{
    _root = new textTreeNode(0);
    currentNode = _root;
}

void textTree::clear()
{
    delete _root;
    _root = 0;
    initialize();
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
    if (currentNode != 0) {
        currentNode = currentNode->parent();
    }
    return currentNode;
}
