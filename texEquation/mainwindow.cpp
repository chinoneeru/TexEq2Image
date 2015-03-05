#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "converter.h"
#include <QPlainTextEdit>
#include <QFileDialog>
#include <QTextStream>
#include <QFile>
#include <QMessageBox>
#include <QThread>
#include <QTimer>
#include <QStringList>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    conv(0),
    convThread(0),
    savePath(""),
    pixmap(),
    scene(),
    autoPreview(true),
    impForm(0),
    packageList(0), includeList(0),
    iniFileKeyPathPlatex("pathPlatex"),
    iniFileKeyPathDvips("pathDvips"),
    iniFileKeyPathImageMagick("pathImageMagick"),
    iniFileKeyPackageList("packageList"),
    iniFileKeyIncludeList("includeList"),
    iniFileKeyLastSaveDir("lastSaveDir")
{
    ui->setupUi(this);
    ui->graphicsViewPreview->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->graphicsViewPreview->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    ui->graphicsViewPreview->setScene(&scene);
    conv = new converter(0, this->ui->comboBoxFont);
    convThread = new QThread();
    conv->moveToThread(convThread);
    convThread->start();

    connect(conv, SIGNAL(convFinished()),
            this, SLOT(enableConvert()),
            Qt::QueuedConnection);

    connect(conv, SIGNAL(convFinished()),
            this, SLOT(enablePreview()),
            Qt::QueuedConnection);

    connect(this, SIGNAL(startConversion()),
            conv, SLOT(execute()),
            Qt::QueuedConnection);

    timer = new QTimer(this);
    timer->setSingleShot(true);
    connect(timer, SIGNAL(timeout()),
            this, SLOT(createPreview()));

    packageList = new QStringList();
    includeList = new QStringList();

    loadIniFile();
}

MainWindow::~MainWindow()
{
    convThread->quit();
    convThread->wait();

    writeIniFile();

    delete timer;
    delete convThread;
    delete impForm;
    delete packageList;
    delete includeList;
    delete ui;
}

void MainWindow::disableConvert()
{
    ui->pushButtonConvert->setEnabled(false);
}

void MainWindow::enableConvert()
{
    ui->pushButtonConvert->setEnabled(true);
}

void MainWindow::disablePreview()
{
    ui->pushButtonPreview->setEnabled(false);
    ui->comboBoxFont->setEnabled(false);
}

void MainWindow::enablePreview()
{
    ui->pushButtonPreview->setEnabled(true);
    ui->comboBoxFont->setEnabled(true);
}

void MainWindow::createPreview()
{
    bool ret = conv->setup(ui->plainTextEditEq->toPlainText(),
                2,
                ui->comboBoxFont->currentIndex(),
                tr("preview"),
                tr("png"),
                *(this->packageList),
                *(this->includeList));

    if(ret != true) return;

    disableConvert();
    disablePreview();

    connect(conv, SIGNAL(convFinished()),
            this, SLOT(showPreview()),
            Qt::QueuedConnection);

    emit startConversion();
}

void MainWindow::showPreview()
{
    disconnect(conv, SIGNAL(convFinished()),
               this, SLOT(showPreview()));

    ui->graphicsViewPreview->setUpdatesEnabled(false);
    scene.clear();

    if(!conv->isSuccessed()) goto finish;
    if(!pixmap.load("preview.png")) goto finish;

    scene.addPixmap(pixmap);
    scene.setSceneRect(0, 0, pixmap.size().width(), pixmap.size().height());

finish:
    ui->graphicsViewPreview->setUpdatesEnabled(true);
}

bool MainWindow::selectDirectory()
{
    QString buf = QFileDialog::getExistingDirectory(this, tr("Select Directory"), this->savePath);

    if(buf == "") {
        return false;
    } else {
        buf.push_back('/');
        savePath = buf;
        return true;
    }
}

void MainWindow::loadIniFile()
{
    QFile iniFile(tr("ini.cfg"));
    if(!iniFile.open(QIODevice::ReadOnly)) return;
    QTextStream in(&iniFile);
    QString buf;
    QStringList keyAndValue;
    while((buf = in.readLine()) != "") {
        keyAndValue = buf.split('=');
        if(keyAndValue[0] == this->iniFileKeyPathPlatex) this->conv->subscribePathPlatex(keyAndValue[1]);//this->pathPlatex = keyAndValue[1];
        if(keyAndValue[0] == this->iniFileKeyPathDvips ) this->conv->subscribePathDvips(keyAndValue[1]);//this->pathDvips  = keyAndValue[1];
        if(keyAndValue[0] == this->iniFileKeyPathImageMagick) this->conv->subscribePathImageMagick(keyAndValue[1]);//this->pathImageMagick = keyAndValue[1];
        if(keyAndValue[0] == this->iniFileKeyPackageList) this->packageList->push_back(keyAndValue[1]);
        if(keyAndValue[0] == this->iniFileKeyIncludeList) this->includeList->push_back(keyAndValue[1]);
        if(keyAndValue[0] == this->iniFileKeyLastSaveDir) this->savePath = keyAndValue[1];
    }
    iniFile.close();
}

void MainWindow::writeIniFile()
{
    QFile iniFile(tr("ini.cfg"));
    if(!iniFile.open(QIODevice::WriteOnly)) return;
    QTextStream out(&iniFile);
    out << this->iniFileKeyPathPlatex << '=' << this->conv->getPathPlatex() << endl;
    out << this->iniFileKeyPathDvips  << '=' << this->conv->getPathDvips()  << endl;
    out << this->iniFileKeyPathImageMagick << '=' << this->conv->getPathImageMagick() << endl;
    out << this->iniFileKeyLastSaveDir << '=' << this->savePath << endl;
    for(int i = 0; i < this->packageList->size(); i++){
        out << this->iniFileKeyPackageList << '=' << this->packageList->at(i) << endl;
    }
    for(int i = 0; i < this->includeList->size(); i++){
        out << this->iniFileKeyIncludeList << '=' << this->includeList->at(i) << endl;
    }
    iniFile.close();
}

void MainWindow::saveText()
{
    QFile outFile(tr("%1%2.txt").arg(this->savePath).arg(this->ui->lineEditName->text()));
    if(!outFile.open(QIODevice::WriteOnly)) return;
    QTextStream out(&outFile);
    out << this->ui->plainTextEditEq->toPlainText();
    outFile.close();
}

bool MainWindow::checkIfExist(const QString &fileName)
{
    if(QFile::exists(tr("%1%2.txt")
                    .arg(this->savePath)
                    .arg(fileName)))
    {
        return true;
    } else {
        return false;
    }
}

void MainWindow::on_pushButtonConvert_clicked()
{
    if (!selectDirectory()) return;

    if(checkIfExist(ui->lineEditName->text())) {
        int ret = QMessageBox::warning(this, tr("warning"),
                              tr("specified file already exist. OK to overwrite?"),
                              QMessageBox::Ok | QMessageBox::Cancel,
                              QMessageBox::Cancel);

        if (ret != QMessageBox::Ok) return;
    }

    saveText();

    bool ret = conv->setup(ui->plainTextEditEq->toPlainText(),
                  ui->spinBoxMagnitude->value(),
                  ui->comboBoxFont->currentIndex(),
                  tr("%1%2").arg(savePath).arg(ui->lineEditName->text()),
                  ui->comboBoxImageType->currentText(),
                  *(this->packageList),
                  *(this->includeList));

    if(ret != true) return;

    disableConvert();
    disablePreview();
    emit startConversion();
}

void MainWindow::on_pushButtonPreview_clicked()
{
    createPreview();
}

/*
void MainWindow::on_actionSelect_directory_2_triggered()
{
    selectDirectory();
}
*/

void MainWindow::on_actionLoad_triggered()
{
    QString inFileName = QFileDialog::getOpenFileName(this, tr("Select File"), "", tr("Text files (*.txt)"));
    if(inFileName == "") return;
    QFile inFile(inFileName);
    if(!inFile.open(QIODevice::ReadOnly)) return;
    QTextStream in(&inFile);
    ui->plainTextEditEq->setPlainText(in.readAll());
    inFile.close();
}

void MainWindow::on_actionDvips_triggered()
{
    conv->subscribePathDvips(QFileDialog::getExistingDirectory(this, tr("Select Directory")));
}

void MainWindow::on_actionPlatex_triggered()
{
    conv->subscribePathPlatex(QFileDialog::getExistingDirectory(this, tr("Select Directory")));
}

void MainWindow::on_actionImageMagick_triggered()
{
    conv->subscribePathImageMagick(QFileDialog::getExistingDirectory(this, tr("Select Directory")));
}

void MainWindow::on_plainTextEditEq_textChanged()
{
    if(!this->autoPreview) return;

    int sourceLength = this->ui->plainTextEditEq->toPlainText().length();
    if(sourceLength == 0) return;

    if (this->ui->plainTextEditEq->getLastEnteredChar() == Qt::Key_Return) {
        this->timer->start(10);
    } else {
        this->timer->start(2000);
    }
}

void MainWindow::on_checkBoxAutoPreview_stateChanged(int arg1)
{
    switch(arg1){
    case Qt::Unchecked:
        this->autoPreview = false;
        break;
    case Qt::Checked:
        this->autoPreview = true;
        break;
    default:
        this->autoPreview = false;
        break;
    }
}

void MainWindow::on_actionImport_2_triggered()
{
    this->impForm = new importForm(this, packageList, includeList);

    impForm->setWindowFlags(Qt::Window);
    impForm->setAttribute(Qt::WA_DeleteOnClose);
    impForm->setAttribute(Qt::WA_ShowModal);
    impForm->show();

    impForm = 0;
}

/*
void MainWindow::on_actionSave_text_triggered()
{
    saveText();
}
*/

void MainWindow::on_comboBoxFont_currentIndexChanged(const QString &arg1)
{
    createPreview();
}