#include "textTreeNode.h"
#include <iostream>

textTreeNode::textTreeNode(textTreeNode *parent, int start, int end) :
    _parent(parent),
    _start(start),
    _end(end),
    _startToken(""),
    _endToken(""),
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
    //textTreeNode* ptr = new textTreeNode(this);
    //children.push_back(ptr);
    //return ptr;
    return 0;
}

textTreeNode* textTreeNode::addChilde(textTreeNode *newNode)
{
    children.push_back(newNode);
    return newNode;
}

void textTreeNode::setStartToken(const QString &token, int pos)
{
    _startToken = token;
    setStart(pos);
}

void textTreeNode::setEndToken(const QString &token, int pos)
{
    _endToken = token;
    setEnd(pos);
}

void textTreeNode::setStart(int startPos)
{
    _start = startPos;
    //std::cout << "startToken: " << _startToken.toLatin1().data() << std::endl;
    //std::cout << "startPos  : " << (int)(_start) << std::endl;
}

void textTreeNode::setEnd(int endPos)
{
    _end = endPos;
    //std::cout << "endToken: " << _endToken.toLatin1().data() << std::endl;
    //std::cout << "endPos  : " << (int)(_end) << std::endl;

    _blockClosed = true;
}

void textTreeNode::setTokenToBeChecked(const QString &token/*, int pos*/)
{
    _tokenToBeChecked = token;
    //posOfTokenToBeChecked = pos;
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

QString textTreeNode::startToken() const
{
    return _startToken;
}

QString textTreeNode::endToken() const
{
    return _endToken;
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
