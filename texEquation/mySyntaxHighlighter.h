#ifndef MYSYNTAXHIGHLIGHTER_H
#define MYSYNTAXHIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QTextDocument>
#include <QTextCursor>
#include <QVector>
#include <QColor>
#include <QPlainTextEdit>
#include <QTextCharFormat>

class mySyntaxHighlighter : public QSyntaxHighlighter {

    Q_OBJECT

public:
    mySyntaxHighlighter(QPlainTextEdit* editor);
    virtual ~mySyntaxHighlighter();
    void highlightBlock(const QString& text);

private slots:
    //enum status{coursor
    //void applyFormat(int start, int count);
    //void setFontColor(const QColor& color);
    //void setBackColor(const QColor& color);
    //void setStyle(int start, int count, bool bold, bool italic);
    //void analyze(const QString& text);

private:
    QTextCharFormat formatTexWord, formatNumber;
    int coursorPos;
    QPlainTextEdit* editor;
    class textTree* txtTree;
};

#endif // MYSYNTAXHIGHLIGHTER_H
