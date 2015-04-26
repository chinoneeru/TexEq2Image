#include "textTreeNode.h"
#include <iostream>

textTreeNode::textTreeNode(textTreeNode *parent, int start, int end) :
    _parent(parent),
    _start(start),
    _end(end),
    focusedChilde(0),
    _blockClosed(false)
{
    if(_parent != 0) {
        //std::cout << "parent's depth = " << (int)(_parent->depth()) << std::endl;
        _depth = _parent->depth() + 1;
        _start = _parent->start();
        _end   = _parent->end();
    } else {
        _depth = 0;
    }

    //std::cout << "depth = " << (int)_depth << std::endl;
}

textTreeNode::~textTreeNode()
{
    for(int i = 0; i < children.count(); i++) {
        //std::cout << "delete" << std::endl;
        delete children[i];
        children[i] = 0;
    }
    children.clear();
}

textTreeNode* textTreeNode::addChilde()
{
    textTreeNode* ptr = new textTreeNode(this);
    children.push_back(ptr);
    return ptr;
}

void textTreeNode::setStart(int startPos)
{
    _start = startPos;
}

void textTreeNode::setEnd(int endPos)
{
    _end = endPos;

    _blockClosed = true;
}

int textTreeNode::start() const
{
    return _start;
}

int textTreeNode::end() const
{
    return _end;
}

int textTreeNode::depth() const
{
    return _depth;
}

textTreeNode* textTreeNode::parent() const
{
    return _parent;
}

bool textTreeNode::blockClosed() const
{
    return _blockClosed;
}

textTreeNode* textTreeNode::nextChilde()
{
    textTreeNode* ret;
    if (children.count() > focusedChilde) {
        ret = children[focusedChilde];
        focusedChilde++;
    } else {
        ret = 0;
        focusedChilde = 0;
    }
    return ret;
}

/*void textTreeNode::clearChildren()
{
    for(int i = 0; i < children.count(); i++) {
        std::cout << "clear" << std::endl;
        delete children[i];
        children[i] = 0;
    }
    children.clear();
}*/
