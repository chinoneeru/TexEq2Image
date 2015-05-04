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
    brushCorrectInside(QColor(0xe7, 0xe7, 0xeb)),
    brushCorrectSides(QColor(0xc8, 0xc2, 0xc6)),
    brushMissInside(QColor(0xf5, 0xb1, 0xaa)),
    brushMissSides(QColor(0xee, 0x82, 0x7c))
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
    QTextEdit::ExtraSelection selection0, selection1, selection2;
    QTextCursor cursor0 = editor->textCursor();
    QTextCursor cursor1 = editor->textCursor();
    QTextCursor cursor2 = editor->textCursor();

    if (highlightedNode->depth() == 0) { /*root-block*/
        if (highlightedNode->blockClosed()) { /*root-block cannot be closed if no mistake occures.*/

            selection0.format.setBackground(brushMissSides);
            selection1.format.setBackground(brushMissInside);
            selection2.format.setBackground(brushMissSides);

        } else {

            selection0.format.setBackground(brushNormal);
            selection1.format.setBackground(brushNormal);
            selection2.format.setBackground(brushNormal);

            goto finish;

        }
    } else {
        if (highlightedNode->blockClosed() &&
                isParenthesisMatched(*highlightedNode)
                ) {

            selection0.format.setBackground(brushCorrectSides);
            selection1.format.setBackground(brushCorrectInside);
            selection2.format.setBackground(brushCorrectSides);

        } else {

            selection0.format.setBackground(brushMissSides);
            selection1.format.setBackground(brushMissInside);
            selection2.format.setBackground(brushMissSides);

        }

    }

    cursor0.setPosition(highlightedNode->start());
    cursor0.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor,
                        highlightedNode->startToken().length());
    cursor1.setPosition(highlightedNode->start() + highlightedNode->startToken().length());
    cursor1.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor,
                         highlightedNode->end() - highlightedNode->start() + 1
                         - highlightedNode->startToken().length() - highlightedNode->endToken().length());
    cursor2.setPosition(highlightedNode->end() - highlightedNode->endToken().length() + 1);
    cursor2.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor,
                        highlightedNode->endToken().length());

finish:

    selection0.cursor = cursor0;
    selection1.cursor = cursor1;
    selection2.cursor = cursor2;

    selections.append(selection0);
    selections.append(selection1);
    selections.append(selection2);

    editor->setExtraSelections(selections);
}

void blockHighlighter::analyze(const QString& text)
{
    txtTree->clear(text.count());
    textTreeNode* node = txtTree->root();

    QString patternLeft = tr("%1|%2|%3|%4|%5|%6|%7")
            .arg("\\\\left[ ]*\\(")
            .arg("\\\\left[ ]*\\[")
            .arg("\\\\left[ ]*\\\\\\{")
            .arg("\\\\left[ ]*\\.")
            .arg("\\\\left[ ]*\\|")
            //.arg("\\(")
            //.arg("\\[")
            .arg("\\\\\\{")
            .arg("\\{")
            ;
    QString patternRight = tr("%1|%2|%3|%4|%5|%6|%7")
            .arg("\\\\right[ ]*\\)")
            .arg("\\\\right[ ]*\\]")
            .arg("\\\\right[ ]*\\\\\\}")
            .arg("\\\\right[ ]*\\.")
            .arg("\\\\right[ ]*\\|")
            //.arg("\\)")
            //.arg("\\]")
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
            if (text.mid(index, tokenLength) != "\\{") {
                node = txtTree->down();
                node->setStartToken(text.mid(index, tokenLength), index);
            }
            break;
        case rightParenthesis:
            tokenLength = expressionRight.matchedLength();
            index = indexRight;
            if (text.mid(index, tokenLength) != "\\}") {
                node->setEndToken(text.mid(index, tokenLength), index);
                node = txtTree->up();
            }
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
