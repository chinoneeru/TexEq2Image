#ifndef BLOCKHIGHLIGHTER_H
#define BLOCKHIGHLIGHTER_H

#include <QObject>
#include <QTextDocument>
#include <QTextCursor>
#include <QVector>
#include <QColor>
#include <QPlainTextEdit>
#include <QBrush>

class blockHighlighter : public QObject {

    Q_OBJECT

public:
    blockHighlighter(QPlainTextEdit* editor);
    virtual ~blockHighlighter();

public slots:
    void highlight();
    void setTextChangedFlag();

private slots:
    void analyze(const QString &text);
    bool isParenthesisMatched(int leftPos, int rightPos, const QString& text);

private:
    //int coursorPos;
    QPlainTextEdit* editor;
    class textTree* txtTree;
    bool textChangedFlag;
    QBrush brushNormal, brushCorrect, brushMiss;
};

#endif // BLOCKHIGHLIGHTER_H
