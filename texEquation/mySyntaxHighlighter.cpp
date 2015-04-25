#include "mySyntaxHighlighter.h"
#include "textTree.h"
#include <QTextCharFormat>
#include <iostream>

mySyntaxHighlighter::mySyntaxHighlighter(QTextDocument *parent, QPlainTextEdit* editor) :
    QSyntaxHighlighter(parent),
    coursorPos(0),
    editor(editor)
{
    connect(editor, SIGNAL(cursorPositionChanged()),
            this, SLOT(updateCursorPos()));

    colors.push_back(QColor(0, 0, 0));
    colors.push_back(QColor(0, 0, 128));
    colors.push_back(QColor(0, 128, 0));
    colors.push_back(QColor(128, 0, 0));

    for(int i = 0; i < colors.count(); i++){
        formats.push_back(new QTextCharFormat());
        formats.at(i)->setForeground(colors.at(i));
    }

    txtTree = new textTree();
}

mySyntaxHighlighter::~mySyntaxHighlighter()
{
    for(int i = 0; i < formats.count(); i++){
        delete formats[i];
    }
    formats.clear();

    delete txtTree;
}

void mySyntaxHighlighter::highlightBlock(const QString &text)
{
    /*analyze(text);

    int _coursorPos = coursorPos - currentBlock().position();

    textTreeNode *node = txtTree->root();
    textTreeNode *nextNode = 0;
    textTreeNode *highlightedNode = node;
    int maxDepth = 0;

    while(node != 0) {

        if (node->start() <= _coursorPos && _coursorPos <= node->end()) {
            if(node->depth() > maxDepth) {
                highlightedNode = node;
                maxDepth = node->depth();
            }
        }

        nextNode = node->nextChilde();
        if (nextNode != 0) {
            node = nextNode;
        } else {
            node = node->parent();
        }
    }

    if (maxDepth == 0) return;

    QTextCharFormat format;
    format.setBackground(Qt::cyan);
    setFormat(
                highlightedNode->start(),
                highlightedNode->end() - highlightedNode->start() + 1,
                format
                );*/
}

/*
void mySyntaxHighlighter::analyze(const QString &text)
{
    txtTree->clear();
    textTreeNode* node = txtTree->root();
    node->setStart(0);
    node->setEnd(text.count()-1);

    for(int i = 0; i < text.count(); i++) {
        switch(text.at(i).toLatin1()) {
        case '(':
        case '{':
        case '[':
            node = txtTree->down();
            node->setStart(i);
            break;
        case ')':
        case '}':
        case ']':
            node->setEnd(i);
            node = txtTree->up();
            break;
        }
    }
}
*/

void mySyntaxHighlighter::updateCursorPos()
{
    coursorPos = editor->textCursor().position();
    std::cout << "coursor position: " << (int)coursorPos << std::endl;
    rehighlight();
    //rehighlightBlock(currentBlock());
}
