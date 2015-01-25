#include "complementer.h"

complementer::complementer(QObject *parent) :
    QObject(parent),
    cursor()
{

}

complementer::~complementer()
{

}

void complementer::start(char lastInputedChar, QPlainTextEdit *textEdit)
{
    QString insert;
    switch(lastInputedChar){
    case '{':
        insert = tr("}");
        break;
    default:
        return;
    }
    textEdit->insertPlainText(insert);
    cursor = textEdit->textCursor();
    cursor.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor, insert.length());
    textEdit->setTextCursor(cursor);
}
