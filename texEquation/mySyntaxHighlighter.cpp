#include "mySyntaxHighlighter.h"
#include <QTextCharFormat>

mySyntaxHighlighter::mySyntaxHighlighter(QTextDocument *parent)
    :QSyntaxHighlighter(parent)
{
    QVector<QColor> colors;
    colors.push_back(QColor(0, 0, 0));
    colors.push_back(QColor(0, 0, 128));
    colors.push_back(QColor(0, 128, 0));
    colors.push_back(QColor(128, 0, 0));

    for(int i = 0; i < colors.count(); i++){
        formats.push_back(new QTextCharFormat());
        formats.at(i)->setForeground(colors.at(i));
    }
}

mySyntaxHighlighter::~mySyntaxHighlighter()
{
    for(int i = 0; i < formats.count(); i++){
        delete formats[i];
    }
    formats.clear();
}

void mySyntaxHighlighter::highlightBlock(const QString &text)
{
    unsigned int depth = 0;

    for(int i = 0; i < text.length(); i++){

        switch(text.at(i).toLatin1()){
        case '(':
        case '{':
        case '[':
            depth++;
            break;
        }

        setFormat(i, 1, *(formats.at(depth % formats.count())));

        switch(text.at(i).toLatin1()){
        case ')':
        case '}':
        case ']':
            depth--;
            break;
        }
    }
}
