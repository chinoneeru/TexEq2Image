#include "mySyntaxHighlighter.h"
#include "textTree.h"
#include <iostream>

mySyntaxHighlighter::mySyntaxHighlighter(QPlainTextEdit* editor) :
    QSyntaxHighlighter(editor->document()),
    coursorPos(0),
    editor(editor)
{
    formatTexWord.setForeground(Qt::darkBlue);
    formatTexWord.setFontWeight(QFont::Bold);
    formatNumber.setForeground(Qt::darkMagenta);
}

mySyntaxHighlighter::~mySyntaxHighlighter()
{
}

void mySyntaxHighlighter::highlightBlock(const QString &text)
{
    QString pattern = "\\\\[A-Za-z]+|\\(|\\)|\\{|\\}|\\[|\\]";

    QRegExp expression(pattern);
    int index = text.indexOf(expression);
    while (index >= 0) {
        int length = expression.matchedLength();
        setFormat(index, length, formatTexWord);
        index = text.indexOf(expression, index + length);
    }

    pattern = "[0-9]+";
    expression.setPattern(pattern);
    index = text.indexOf(expression);
    while (index >= 0) {
        int length = expression.matchedLength();
        setFormat(index, length, formatNumber);
        index = text.indexOf(expression, index + length);
    }
}

