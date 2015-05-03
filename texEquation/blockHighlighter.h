#ifndef BLOCKHIGHLIGHTER_H
#define BLOCKHIGHLIGHTER_H

#include <QObject>
#include <QTextDocument>
#include <QTextCursor>
#include <QVector>
#include <QColor>
#include <QPlainTextEdit>
#include <QBrush>

class blockHighlighter : public QObject {

    Q_OBJECT

public:
    blockHighlighter(QPlainTextEdit* editor);
    virtual ~blockHighlighter();

public slots:
    void highlight();
    void setTextChangedFlag();
    class textTreeNode* highlightedBlock();

private slots:
    void analyze(const QString& text);
    bool isParenthesisMatched(const class textTreeNode& highlightedNode);
    int findParenthesisToken(const QString& text, int startPos);

private:
    enum firstParenthesis {leftParenthesis, rightParenthesis};

private:
    //int coursorPos;
    QPlainTextEdit* editor;
    class textTree* txtTree;
    class textTreeNode* highlightedNode;
    //class textTreeNode* dummyNode;
    bool textChangedFlag;
    QBrush brushNormal, brushCorrect, brushMiss;
};

#endif // BLOCKHIGHLIGHTER_H
