#ifndef TEXTTREENODE_H
#define TEXTTREENODE_H

#include <QVector>
#include <QString>

class textTreeNode {

public:
    textTreeNode(class textTreeNode* parent = 0, int start = 0, int end = 0);
    ~textTreeNode();
    textTreeNode *addChilde();
    void setStart(int startPos);
    void setEnd(int endPos);
    void setStartToken(const QString& token, int pos);
    void setEndToken(const QString& token, int pos);
    int start() const;
    int end() const;
    QString startToken() const;
    QString endToken() const;
    int depth() const;
    bool blockClosed() const;
    textTreeNode* parent() const;
    textTreeNode* nextChilde();
    //void clearChildren();

private:
    int _depth;
    int _start;
    int _end;
    QString _startToken;
    QString _endToken;
    int focusedChilde;
    class textTreeNode* _parent;
    QVector<class textTreeNode*> children;
    bool _blockClosed;
};

#endif // TEXTTREENODE_H
