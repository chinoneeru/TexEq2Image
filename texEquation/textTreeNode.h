#ifndef TEXTTREENODE_H
#define TEXTTREENODE_H

#include <QVector>

class textTreeNode {

public:
    textTreeNode(class textTreeNode* parent = 0);
    ~textTreeNode();
    textTreeNode *addChilde();
    void setStart(int startPos);
    void setEnd(int endPos);
    int start() const;
    int end() const;
    int depth() const;
    textTreeNode* parent() const;
    textTreeNode* nextChilde();
    //void clearChildren();

private:
    int _depth;
    int _start;
    int _end;
    int focusedChilde;
    class textTreeNode* _parent;
    QVector<class textTreeNode*> children;
};

#endif // TEXTTREENODE_H
