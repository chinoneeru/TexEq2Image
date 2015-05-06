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
    int coursorPos = editor->textCursor().position();

    if (wasTextChanged(text)) {
        analyze(text);
    }

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
        if (highlightedNode->blockClosed() /*&&
                isParenthesisMatched(*highlightedNode)*/
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

    //std::cout << "cursor0" << std::endl;
    //std::cout << "cursor0:set: " << (int)(highlightedNode->start()) << std::endl;
    //std::cout << "cursor0:mov: " << (int)(highlightedNode->startToken().length()) << std::endl;
    cursor0.setPosition(highlightedNode->start());
    cursor0.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor,
                        highlightedNode->startToken().length());

    //std::cout << "cursor1" << std::endl;
    //std::cout << "cursor1:set: " << (int)(highlightedNode->start() + highlightedNode->startToken().length()) << std::endl;
    //std::cout << "cursor1:mov: " << (int)(highlightedNode->end() - highlightedNode->start() + 1
    //                                      - highlightedNode->startToken().length() - highlightedNode->endToken().length()) << std::endl;
    cursor1.setPosition(highlightedNode->start() + highlightedNode->startToken().length());
    cursor1.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor,
                         highlightedNode->end() - highlightedNode->start()
                         - highlightedNode->startToken().length());

    //std::cout << "cursor2" << std::endl;
    //std::cout << "cursor2:set: " << (int)(highlightedNode->end() - highlightedNode->endToken().length() + 1) << std::endl;
    //std::cout << "cursor2:mov: " << (int)(highlightedNode->endToken().length()) << std::endl;
    cursor2.setPosition(highlightedNode->end());
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

    //textTreeNode* node = txtTree->root();

    QString patterns = tr("%1|%2|%3|%4|%5|%6|%7|%8|%9|%10|%11|%12|%13|%14|%15|%16|%17|%18|%19")
            .arg("\\\\right\\s*\\)")
            .arg("\\\\right\\s*\\]")
            .arg("\\\\right\\s*\\\\\\}")
            .arg("\\\\right\\s*\\.")
            .arg("\\\\rights\\s*\\|")
            .arg("\\\\left\\s*\\(")
            .arg("\\\\left\\s*\\[")
            .arg("\\\\left\\s*\\\\\\{")
            .arg("\\\\left\\s*\\.")
            .arg("\\\\left\\s*\\|")
            .arg("\\\\textrm(?![a-zA-Z]+)")
            .arg("\\\\sqrt(?![a-zA-Z]+)")
            .arg("\\\\frac(?![a-zA-Z]+)")
            .arg("\\\\\\{")
            .arg("\\\\\\}")
            .arg("\\{")
            .arg("\\}")
            .arg("\\\\[a-zA-Z]+")
            .arg("[a-zA-Z0-9]")
            ;

    //QRegExp expressionLeft(patternLeft);
    //QRegExp expressionRight(patternRight);
    QRegExp expression(patterns);

    //int searchStartPos = 0;
    //int tokenLength = 0;
    int index = text.indexOf(expression, 0);

    while(index >= 0) {
        txtTree->setToken(text.mid(index, expression.matchedLength()), index);
        index = text.indexOf(expression, index + expression.matchedLength());
    }
}

bool blockHighlighter::wasTextChanged(const QString &text)
{
    static int oldTextLength = 0;

    if (text.length() != oldTextLength) {
        oldTextLength = text.length();
        return true;
    } else {
        return false;
    }
}

textTreeNode *blockHighlighter::highlightedBlock()
{
    return highlightedNode;
}
