#include "blockHighlighter.h"
#include "textTree.h"
#include <QList>
#include <QTextCharFormat>
#include <iostream>

blockHighlighter::blockHighlighter(QPlainTextEdit* editor) :
    editor(editor),
    txtTree(0),
    highlightedNode(0),// dummyNode(0),
    textChangedFlag(false),
    brushNormal(QColor(0xff, 0xff, 0xff)),
    brushCorrect(QColor(0xe4, 0xd2, 0xd8)),
    brushMiss(QColor(0xf5, 0xb1, 0xaa))
{
    txtTree = new textTree();

    connect(this->editor, SIGNAL(textChanged()),
            this, SLOT(setTextChangedFlag()));

    connect(this->editor, SIGNAL(cursorPositionChanged()),
            this, SLOT(highlight()));

    //dummyNode = new textTreeNode();
}

blockHighlighter::~blockHighlighter()
{
    delete txtTree;
    //delete dummyNode;
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
    highlightedNode = node;
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
            selection.format.setBackground(brushMiss);
            cursor.setPosition(highlightedNode->start());
            cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor,
                                highlightedNode->end() - highlightedNode->start() + 1);
        } else {
            selection.format.setBackground(brushNormal);
        }
    } else {
        if (highlightedNode->blockClosed() &&
                isParenthesisMatched(*highlightedNode)
                ) {
            selection.format.setBackground(brushCorrect);
        } else {
            selection.format.setBackground(brushMiss);
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

    QString patternLeft = tr("%1|%2|%3|%4|%5|%6|%7|%8|%9")
            .arg("\\\\left[ ]*\\(")
            .arg("\\\\left[ ]*\\[")
            .arg("\\\\left[ ]*\\\\\\{")
            .arg("\\\\left[ ]*\\.")
            .arg("\\\\left[ ]*\\|")
            .arg("\\(")
            .arg("\\[")
            .arg("\\\\\\{")
            .arg("\\{")
            ;
    QString patternRight = tr("%1|%2|%3|%4|%5|%6|%7|%8|%9")
            .arg("\\\\right[ ]*\\)")
            .arg("\\\\right[ ]*\\]")
            .arg("\\\\right[ ]*\\\\\\}")
            .arg("\\\\right[ ]*\\.")
            .arg("\\\\right[ ]*\\|")
            .arg("\\)")
            .arg("\\]")
            .arg("\\\\\\}")
            .arg("\\}")
            ;

    QRegExp expressionLeft(patternLeft);
    QRegExp expressionRight(patternRight);

    int searchStartPos = 0;
    int tokenLength = 0;
    int index = 0;

    while(1) {
        firstParenthesis parenthesis;

        int indexLeft =  text.indexOf(expressionLeft , searchStartPos);
        int indexRight = text.indexOf(expressionRight, searchStartPos);

        if (indexLeft < 0 && indexRight < 0) {
            break;
        } else if (indexLeft < 0) {
            parenthesis = rightParenthesis;
        } else if (indexRight < 0) {
            parenthesis = leftParenthesis;
        } else {
            if (indexLeft < indexRight) {
                parenthesis = leftParenthesis;
            } else {
                parenthesis = rightParenthesis;
            }
        }

        switch (parenthesis) {
        case leftParenthesis:
            tokenLength = expressionLeft.matchedLength();
            index = indexLeft;
            node = txtTree->down();
            node->setStartToken(text.mid(index, tokenLength), index);
            break;
        case rightParenthesis:
            tokenLength = expressionRight.matchedLength();
            index = indexRight;
            node->setEndToken(text.mid(index, tokenLength), index);
            node = txtTree->up();
            break;
        }

        searchStartPos = index + tokenLength;
    }
}

void blockHighlighter::setTextChangedFlag()
{
    textChangedFlag = true;
    highlight();
}

bool blockHighlighter::isParenthesisMatched(const textTreeNode& highlightedNode)
{
    bool ret = true;

    //std::cout << highlightedNode.startToken().toLatin1().data() << std::endl;
    //std::cout << highlightedNode.endToken().toLatin1().data() << std::endl;

    if (highlightedNode.startToken() == "{") {
        ret = false;
        if (highlightedNode.endToken() == "}") {
            ret = true;
        }
    } else if (highlightedNode.startToken().contains("\\left")) {
        ret = false;
        if (highlightedNode.endToken().contains("\\right")) {
            ret = true;
        }
    }

    return ret;
}

int blockHighlighter::findParenthesisToken(const QString &text, int startPos)
{
    return 0;
}

textTreeNode *blockHighlighter::highlightedBlock()
{
    return highlightedNode;
}
