#ifndef MYSYNTAXHIGHLIGHTER_H
#define MYSYNTAXHIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QTextDocument>
#include <QVector>

class mySyntaxHighlighter : public QSyntaxHighlighter {

    Q_OBJECT

public:
    mySyntaxHighlighter(QTextDocument * parent);
    virtual ~mySyntaxHighlighter();
    void highlightBlock(const QString& text);

private:
    //void analyze();

private:
    QVector<QTextCharFormat*> formats;

};

#endif // MYSYNTAXHIGHLIGHTER_H
