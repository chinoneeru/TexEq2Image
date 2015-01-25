#ifndef MYPLAINTEXTEDIT_H
#define MYPLAINTEXTEDIT_H

#include <QObject>
#include <QPlainTextEdit>
#include "complementer.h"

class myPlainTextEdit : public QPlainTextEdit {
    Q_OBJECT

public:
    myPlainTextEdit(QWidget *parent = 0);
    ~myPlainTextEdit();

public slots:
    int getLastEnteredChar() const;

protected slots:
    virtual void keyPressEvent(QKeyEvent *event);

private slots:
    void completion(QKeyEvent *event);
    bool simpleEmacsEmulator(QKeyEvent *event);
    void textEnhanced(bool enhanced);
    void bracketHighlighter();
    void bracketHighlighter(bool fragEnhance);
    void enhanceCharAtCursor(bool enhance);
    bool moveCursorToComplementaryBracket(char bracket, int cursorPos);

signals:
    void startInputCompletion(char lastInputedChar, QPlainTextEdit *textEdit);

private:
    QTextCursor cursor;
    QTextCharFormat formatNormal, formatEnhanced;
    int lastEnteredChar;
    //complementer *inputComp;
};

#endif // MYPLAINTEXTEDIT_H
