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
    class textTreeNode* highlightedBlock();

private slots:
    void analyze(const QString& text);
    bool wasTextChanged(const QString& text);

private:
    enum firstParenthesis {leftParenthesis, rightParenthesis};

private:
    //int coursorPos;
    QPlainTextEdit* editor;
    class textTree* txtTree;
    class textTreeNode* highlightedNode;
    //class textTreeNode* dummyNode;
    bool textChangedFlag;
    QBrush brushNormal, brushCorrectInside, brushCorrectSides, brushMissInside, brushMissSides;
};

#endif // BLOCKHIGHLIGHTER_H
