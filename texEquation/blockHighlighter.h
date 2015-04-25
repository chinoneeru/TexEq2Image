#ifndef BLOCKHIGHLIGHTER_H
#define BLOCKHIGHLIGHTER_H

#include <QObject>
#include <QTextDocument>
#include <QTextCursor>
#include <QVector>
#include <QColor>
#include <QPlainTextEdit>

class blockHighlighter : public QObject {

    Q_OBJECT

public:
    blockHighlighter(QPlainTextEdit* editor);
    virtual ~blockHighlighter();

public slots:
    void highlight();
    void setTextChangedFlag();

private slots:
    void analyze();

private:
    QVector<QColor> colors;
    QVector<QTextCharFormat*> formats;
    //int coursorPos;
    QPlainTextEdit* editor;
    class textTree* txtTree;
    bool textChangedFlag;
};

#endif // BLOCKHIGHLIGHTER_H
