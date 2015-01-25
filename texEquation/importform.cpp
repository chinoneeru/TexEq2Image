#include "importform.h"
#include "ui_importform.h"
#include <iostream>
#include <QMessageBox>
#include <QFile>

importForm::importForm(QWidget *parent, QStringList *packageList, QStringList *includeList) :
    QWidget(parent),
    ui(new Ui::importForm),
    packageList(packageList), includeList(includeList)
{
    ui->setupUi(this);

    setFixedSize(this->size());

    for(int i = 0; i < this->packageList->size(); i++){
        ui->plainTextEditPackageList->insertPlainText(this->packageList->at(i) + "\n");
    }
    for(int i = 0; i < this->includeList->size(); i++){
        ui->plainTextEditIncludeList->insertPlainText(this->includeList->at(i) + "\n");
    }
}

importForm::~importForm()
{
    delete ui;
}

void importForm::closeEvent(QCloseEvent *event)
{
    saveStringByLine(ui->plainTextEditPackageList->toPlainText(), packageList);
    saveStringByLine(ui->plainTextEditIncludeList->toPlainText(), includeList);

    if (!verifyIfAllFilesExist()) {
        event->ignore();
    } else {
        event->accept();
    }
}

bool importForm::verifyIfAllFilesExist()
{
    QFile file;

    /*for (int i = 0; i < packageList->size(); i++) {
        file.setFileName(packageList->at(i));
        if (!file.exists()) {
            goto errorFinish;
        }
    }*/

    for (int i = 0; i < includeList->size(); i++) {
        file.setFileName(includeList->at(i));
        if (!file.exists()) {
            goto errorFinish;
        }
    }

    return true;

errorFinish:
    QMessageBox::about(this, tr("file not found"), tr("check the name below:\n%1").arg(file.fileName()));
    return false;
}

void importForm::saveStringByLine(const QString &text, QStringList *list)
{
    QStringList temp = text.split(QRegExp("\n+"), QString::SkipEmptyParts);

    list->clear();
    for (int i = 0; i < temp.size(); i++) {
        list->push_back(temp.at(i));
    }
}
