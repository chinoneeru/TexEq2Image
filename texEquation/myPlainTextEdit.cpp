#include "myPlainTextEdit.h"
#include "blockHighlighter.h"
#include "mySyntaxHighlighter.h"
#include "textTreeNode.h"
#include <QMessageBox>
#include <iostream>

myPlainTextEdit::myPlainTextEdit(QWidget *parent) :
    QPlainTextEdit(parent),
    cursor(),
    formatNormal(),
    formatEnhanced(),
    lastEnteredChar('\0'),
    blkHighlighter(0),
    myHighlighter(0)
{
    /*inputComp = new complementer(this);
    connect(this, SIGNAL(startInputCompletion(char,QPlainTextEdit*)),
            this->inputComp, SLOT(start(char,QPlainTextEdit*)));*/
    formatNormal = this->currentCharFormat();
    formatEnhanced = this->currentCharFormat();
    formatEnhanced.setFontUnderline(true);

    blkHighlighter = new blockHighlighter(this);
    myHighlighter = new mySyntaxHighlighter(this);
}

myPlainTextEdit::~myPlainTextEdit()
{
    //delete inputComp; inputComp = 0;
    delete blkHighlighter;
}

int myPlainTextEdit::getLastEnteredChar() const
{
    return this->lastEnteredChar;
}

bool myPlainTextEdit::getHighlightedArea(QString* startToken, int* startTokenPos, QString* endToken, int* endTokenPos)
{
    textTreeNode* node = blkHighlighter->highlightedBlock();
    if(node == 0) {
        return false;
    } else {
        if (node->depth() > 0) {
            *startToken = node->startToken();
            *endToken   = node->endToken();
            *startTokenPos = node->start();
            *endTokenPos   = node->end();
            return true;
        } else {
            return false;
        }
    }
}

void myPlainTextEdit::keyPressEvent(QKeyEvent *event)
{
    //QMessageBox::about(0, tr(""), tr("%1").arg(this->overwriteMode()));
    //bracketHighlighter(false);
    this->lastEnteredChar = event->key();

    //std::cout << "keyPressEvent" << this->currentCharFormat().fontUnderline() << std::endl;

    if (event->modifiers().testFlag(Qt::ControlModifier)) {
        if(!simpleEmacsEmulator(event) && !specialCommands(event)){
            QPlainTextEdit::keyPressEvent(event);
        }
    } else {
        QPlainTextEdit::keyPressEvent(event);
        completion(event);
    }
}

void myPlainTextEdit::completion(QKeyEvent *event)
{
    QString insert;
    switch(event->key()){
    case '{':
        insert = tr("}");
        break;
    case '(':
        insert = tr(")");
        break;
    case '[':
        insert = tr("]");
        break;
    default:
        return;
    }

    //this->blockSignals(true);
    this->insertPlainText(insert);
    cursor = this->textCursor();
    cursor.movePosition(QTextCursor::Left, QTextCursor::MoveAnchor, insert.length());
    this->setTextCursor(cursor);
    //this->blockSignals(false);

    //emit textChanged();
    //emit cursorPositionChanged();
}

bool myPlainTextEdit::simpleEmacsEmulator(QKeyEvent *event)
{
    //QTextCursor::MoveOperation moveOperation = QTextCursor::Right;
    cursor = this->textCursor();

    switch (event->key()) {
    case Qt::Key_F:
        cursor.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, 1);
        break;
    case Qt::Key_B:
        cursor.movePosition(QTextCursor::Left, QTextCursor::MoveAnchor, 1);
        break;
    case Qt::Key_P:
        cursor.movePosition(QTextCursor::Up, QTextCursor::MoveAnchor, 1);
        break;
    case Qt::Key_N:
        cursor.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor, 1);
        break;
    case Qt::Key_E:
        cursor.movePosition(QTextCursor::EndOfLine, QTextCursor::MoveAnchor, 1);
        break;
    case Qt::Key_A:
        cursor.movePosition(QTextCursor::StartOfLine, QTextCursor::MoveAnchor, 1);
        break;
    case Qt::Key_D:
        cursor.deleteChar();
        break;
    default:
        goto nothingToDo;
    }

    this->setTextCursor(cursor);

    return true;

nothingToDo:
    return false;
}

bool myPlainTextEdit::specialCommands(QKeyEvent *event)
{
    bool ret = false;

    switch(event->key()) {
    case Qt::Key_H:
        emit highlightedPreviewCommand();
        ret = true;
        break;
    }

    return ret;
}

/*
void myPlainTextEdit::textEnhanced(bool enhanced)
{
    //cursor.movePosition(QTextCursor::Left, QTextCursor::MoveAnchor, 1);
    if(enhanced) {
        std::cout << "enhance" << std::endl;
        //this->setCurrentCharFormat(formatEnhanced);
        (cursor = this->textCursor()).setCharFormat(formatEnhanced);
    } else {
        std::cout << "unenhance" << std::endl;
        //this->setCurrentCharFormat(formatNormal);
        (cursor = this->textCursor()).setCharFormat(formatNormal);
    }

    this->setTextCursor(cursor);

    std::cout << "format change" << this->currentCharFormat().fontUnderline() << std::endl;
    //cursor.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, 1);
}

void myPlainTextEdit::bracketHighlighter()
{
    bracketHighlighter(true);
}

void myPlainTextEdit::bracketHighlighter(bool fragEnhance)
{
    QTextCursor initialCursor = this->textCursor();

    char charAtCursor = '\0';

    if (0 < initialCursor.position() && initialCursor.position() <= this->toPlainText().length()){
        charAtCursor = this->toPlainText().at(initialCursor.position() - 1).toLatin1();
    }
    std::cout << "charAtCursor = " << charAtCursor << "; position = " << initialCursor.position() << std::endl;

    switch(charAtCursor){
    case '{':
    case '}':
    case '(':
    case ')':
    case '[':
    case ']':
        this->blockSignals(true);
        enhanceCharAtCursor(fragEnhance);
        if(moveCursorToComplementaryBracket(charAtCursor, initialCursor.position() - 1)){
            enhanceCharAtCursor(fragEnhance);
            //this->setTextCursor(initialCursor);
            std::cout << "returned" << std::endl;
        }
        this->setTextCursor(initialCursor);
        this->blockSignals(false);
        break;
    default:
        break;
    }
}

void myPlainTextEdit::enhanceCharAtCursor(bool enhance)
{
    char charAtCursor = this->toPlainText().at(this->textCursor().position() - 1).toLatin1();
    this->textCursor().deletePreviousChar();

    textEnhanced(enhance);
    this->insertPlainText(tr("%1").arg(charAtCursor));
    textEnhanced(false);
}

#define SEARCH_LEFT  -1
#define SEARCH_RIGHT 1

bool myPlainTextEdit::moveCursorToComplementaryBracket(char bracket, int cursorPos)
{
    char counterBracket = '\0';
    int searchDirection = SEARCH_LEFT;

    switch (bracket) {
    case '{':
        counterBracket = '}';
        searchDirection = SEARCH_RIGHT;
        break;
    case '}':
        counterBracket = '{';
        searchDirection = SEARCH_LEFT;
        break;
    default:
        return false;
    }

    QString text = this->toPlainText();
    int bracketCounter = 0;
    bool correctCounterBracketFound = false;

    std::cout << "text.at(cursorPos) = " << text.at(cursorPos).toLatin1() << "; position = " << cursorPos << std::endl;

    int newCursorPos = cursorPos;
    for(; newCursorPos < text.length() && newCursorPos >= 0; newCursorPos += searchDirection){
        if (text.at(newCursorPos) == bracket) {
            std::cout << "bracket found" << std::endl;
            bracketCounter++;
        } else if (text.at(newCursorPos) == counterBracket) {
            std::cout << "counter bracket found" << std::endl;
            bracketCounter--;
        }

        if (bracketCounter == 0) {
            correctCounterBracketFound = true;
            break;
        }
    }

    if (correctCounterBracketFound) {
        QTextCursor tempCursor = this->textCursor();
        tempCursor.movePosition((searchDirection == SEARCH_RIGHT) ? QTextCursor::Right : QTextCursor::Left,
                                QTextCursor::MoveAnchor,
                                (cursorPos < newCursorPos) ? newCursorPos - cursorPos : cursorPos - newCursorPos);
        this->setTextCursor(tempCursor);
        return true;
    } else {
        return false;
    }
}
*/
