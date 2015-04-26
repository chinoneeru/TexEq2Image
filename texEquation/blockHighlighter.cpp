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

    connect(this->editor, SIGNAL(textChanged()),
            this, SLOT(setTextChangedFlag()));

    connect(this->editor, SIGNAL(cursorPositionChanged()),
            this, SLOT(highlight()));
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
    QString text = editor->toPlainText();

    if (textChangedFlag) {
        analyze(text);
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

    if (highlightedNode->depth() == 0) { /*root-block*/
        if (highlightedNode->blockClosed()) { /*root-block cannot be closed if no mistake occures.*/
            selection.format.setBackground(Qt::magenta);
            cursor.setPosition(highlightedNode->start());
            cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor,
                                highlightedNode->end() - highlightedNode->start() + 1);
        } else {
            selection.format.setBackground(Qt::white);
        }
    } else {
        if (highlightedNode->blockClosed() &&
                isParenthesisMatched(highlightedNode->start(), highlightedNode->end(), text)
                ) {
            selection.format.setBackground(Qt::cyan);
        } else {
            selection.format.setBackground(Qt::magenta);
        }
        cursor.setPosition(highlightedNode->start());
        cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor,
                            highlightedNode->end() - highlightedNode->start() + 1);
    }

    selection.cursor = cursor;
    selections.append(selection);
    editor->setExtraSelections(selections);
}

void blockHighlighter::analyze(const QString& text)
{
    txtTree->clear(text.count());
    textTreeNode* node = txtTree->root();

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
    highlight();
}

bool blockHighlighter::isParenthesisMatched(int leftPos, int rightPos, const QString& text)
{
    char l = text.at(leftPos).toLatin1();
    char r = text.at(rightPos).toLatin1();

    //std::cout << "l = " << l << std::endl;
    //std::cout << "r = " << r << std::endl;

    switch(l) {
    case '(':
        if (r == ')') return true;
        break;
    case '{':
        if (r == '}') return true;
        break;
    case '[':
        if (r == ']') return true;
        break;
    default:
        return true;
        break;
    }

    return false;
}
