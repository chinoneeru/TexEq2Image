#ifndef TEXTTREENODE_H
#define TEXTTREENODE_H

#include <QVector>
#include <QString>

class textTreeNode {

public:
    textTreeNode(class textTreeNode* parent = 0, int start = 0, int end = 0);
    virtual ~textTreeNode();
    textTreeNode *addChilde();
    textTreeNode *addChilde(class textTreeNode* newNode);
    void setStart(int startPos);
    void setEnd(int endPos);
    void setStartToken(const QString& token, int pos);
    void setEndToken(const QString& token, int pos);
    void setTokenToBeChecked(const QString& token);
    int start() const;
    int end() const;
    QString startToken() const;
    QString endToken() const;
    int depth() const;
    bool blockClosed() const;
    textTreeNode* parent() const;
    textTreeNode* nextChilde();

public:
    virtual bool isClosingCondition() = 0;

protected:
    QString _tokenToBeChecked;
    int posOfTokenToBeChecked;

protected:
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
