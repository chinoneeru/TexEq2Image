#ifndef COMPLEMENTER_H
#define COMPLEMENTER_H

#include <QObject>
#include <QPlainTextEdit>

class complementer : public QObject {
    Q_OBJECT

public:
    complementer(QObject *parent = 0);
    ~complementer();

public slots:
    void start(char lastInputedChar, QPlainTextEdit *textEdit);

private:
    QTextCursor cursor;
};

#endif // COMPLEMENTER_H
