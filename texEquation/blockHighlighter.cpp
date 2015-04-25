#include "blockHighlighter.h"
#include "textTree.h"
#include <QList>
#include <QTextCharFormat>
#include <iostream>

blockHighlighter::blockHighlighter(QPlainTextEdit* editor) :
    editor(editor),
    textChangedFlag(false)
{
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

blockHighlighter::~blockHighlighter()
{
    for(int i = 0; i < formats.count(); i++){
        delete formats[i];
    }
    formats.clear();

    delete txtTree;
}

void blockHighlighter::highlight()
{
    if (textChangedFlag) {
        analyze();
        textChangedFlag = false;
    }

    int coursorPos = editor->textCursor().position();

    textTreeNode *node = txtTree->root();
    textTreeNode *nextNode = 0;
    textTreeNode *highlightedNode = node;
    int maxDepth = 0;

    while(node != 0) {

        if (node->start() <= coursorPos && coursorPos <= node->end()) {
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

    QList<QTextEdit::ExtraSelection> selections;
    QTextEdit::ExtraSelection selection;
    QTextCursor cursor = editor->textCursor();

    if (highlightedNode->depth() > 0 ) {
        selection.format.setBackground(Qt::cyan);
        //int iniPos = cursor.position();
        cursor.setPosition(highlightedNode->start());
        cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor,
                            highlightedNode->end() - highlightedNode->start() + 1);
    } else {
        selection.format.setBackground(Qt::white);
    }

    selection.cursor = cursor;
    selections.append(selection);
    editor->setExtraSelections(selections);
}

void blockHighlighter::analyze()
{
    QString text = editor->toPlainText();
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

void blockHighlighter::setTextChangedFlag()
{
    textChangedFlag = true;
}
