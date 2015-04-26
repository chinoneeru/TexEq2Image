#include "mySyntaxHighlighter.h"
#include "textTree.h"
#include <iostream>

mySyntaxHighlighter::mySyntaxHighlighter(QPlainTextEdit* editor) :
    QSyntaxHighlighter(editor->document()),
    coursorPos(0),
    editor(editor)
{
    formatTexWord.setForeground(QColor(0x00, 0xa4, 0x97));
    //formatTexWord.setFontWeight(QFont::Bold);
    formatNumber.setForeground(QColor(0xc8, 0x99, 0x32));
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

