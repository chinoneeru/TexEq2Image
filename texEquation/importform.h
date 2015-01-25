#ifndef IMPORTFORM_H
#define IMPORTFORM_H

#include <QWidget>
#include <QStringList>

namespace Ui {
class importForm;
}

class importForm : public QWidget
{
    Q_OBJECT

public:
    explicit importForm(QWidget *parent = 0, QStringList *packageList = 0, QStringList *includeList = 0);
    ~importForm();

private slots:
    bool verifyIfAllFilesExist();
    void saveStringByLine(const QString& text, QStringList *list);
    virtual void closeEvent(QCloseEvent *event);

private slots:

private:
    Ui::importForm *ui;
    QStringList *packageList;
    QStringList *includeList;
};

#endif // IMPORTFORM_H
