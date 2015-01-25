#include "myPlainTextEdit.h"
#include <QMessageBox>
#include <iostream>

myPlainTextEdit::myPlainTextEdit(QWidget *parent) :
    QPlainTextEdit(parent),
    cursor(),
    formatNormal(),
    formatEnhanced(),
    lastEnteredChar('\0')
{
    /*inputComp = new complementer(this);
    connect(this, SIGNAL(startInputCompletion(char,QPlainTextEdit*)),
            this->inputComp, SLOT(start(char,QPlainTextEdit*)));*/
    formatNormal = this->currentCharFormat();
    formatEnhanced = this->currentCharFormat();
    formatEnhanced.setFontUnderline(true);

    /*connect(this, SIGNAL(cursorPositionChanged()),
            this, SLOT(bracketHighlighter()));*/
}

myPlainTextEdit::~myPlainTextEdit()
{
    //delete inputComp; inputComp = 0;
}

int myPlainTextEdit::getLastEnteredChar() const
{
    return this->lastEnteredChar;
}

void myPlainTextEdit::keyPressEvent(QKeyEvent *event)
{
    //QMessageBox::about(0, tr(""), tr("%1").arg(this->overwriteMode()));
    //bracketHighlighter(false);
    this->lastEnteredChar = event->key();

    if (event->modifiers().testFlag(Qt::ControlModifier)) {
        if(!simpleEmacsEmulator(event)){
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

    this->blockSignals(true);
    //textEnhanced(true);
    this->insertPlainText(insert);
    //textEnhanced(false);
    cursor = this->textCursor();
    cursor.movePosition(QTextCursor::Left, QTextCursor::MoveAnchor, insert.length());
    this->setTextCursor(cursor);
    this->blockSignals(false);
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

void myPlainTextEdit::textEnhanced(bool enhanced)
{
    if(enhanced) {
        this->setCurrentCharFormat(formatEnhanced);
    } else {
        this->setCurrentCharFormat(formatNormal);
    }
}

void myPlainTextEdit::bracketHighlighter()
{
    bracketHighlighter(true);
}

void myPlainTextEdit::bracketHighlighter(bool fragEnhance)
{
    QTextCursor initialCursor = this->textCursor();

    //if(initialCursor.atStart()) return;

    char charAtCursor = this->toPlainText().at(initialCursor.position()).toLatin1();
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
        if(moveCursorToComplementaryBracket(charAtCursor, initialCursor.position()-1)){
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
    char charAtCursor = this->toPlainText().at(this->textCursor().position()).toLatin1();
    this->textCursor().deleteChar();
    if(enhance) textEnhanced(true);
    this->insertPlainText(tr("%1").arg(charAtCursor));
    if(enhance) textEnhanced(false);

    QTextCursor tempCursor = this->textCursor();
    tempCursor.movePosition(QTextCursor::Left, QTextCursor::MoveAnchor, 1);
    this->setTextCursor(tempCursor);
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

    //QMessageBox::about(0, tr(""), tr("%1").arg(text.at(cursorPos)));
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
