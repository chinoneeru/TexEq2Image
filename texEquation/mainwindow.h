#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "converter.h"
#include <QString>
#include <QPixmap>
#include <QGraphicsScene>
#include <QPlainTextEdit>
//#include "complementer.h"
#include "importform.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    bool selectDirectory();
    void disableConvert();
    void enableConvert();
    void disablePreview();
    void enablePreview();
    void createPreview();
    void showPreview();
    void loadIniFile();
    void writeIniFile();
    void saveText();
    bool checkIfExist(const QString& fileName);
    void setFontList(const QString& font);
    void resizeEvent(QResizeEvent *);

private slots:
    void on_pushButtonConvert_clicked();

    void on_pushButtonPreview_clicked();

    //void on_actionSelect_directory_2_triggered();

    void on_actionLoad_triggered();

    void on_actionDvips_triggered();

    void on_actionPlatex_triggered();

    void on_actionDviPng_triggered();

    void on_plainTextEditEq_textChanged();

    void on_checkBoxAutoPreview_stateChanged(int arg1);

    void on_actionImport_2_triggered();

    //void on_actionSave_text_triggered();

    void on_comboBoxFont_currentIndexChanged(const QString &arg1);

    //void on_comboBoxFont_currentTextChanged(const QString &arg1);

    void on_actionFont_triggered();

signals:
    void startConversion();

private:
    Ui::MainWindow *ui;
    converter *conv;
    QThread *convThread;
    QString savePath;
    QPixmap pixmap;
    QGraphicsScene scene;
    QTimer *timer;
    bool autoPreview;
    importForm *impForm;
    QStringList *packageList, *includeList;
    class mySyntaxHighlighter* highlighter;
//    complementer *inputComp;

private:
    const QString iniFileKeyPathPlatex;
    const QString iniFileKeyPathDvips;
    const QString iniFileKeyPathDvipng;
    const QString iniFileKeyPackageList;
    const QString iniFileKeyIncludeList;
    const QString iniFileKeyLastSaveDir;
    const QString iniFileKeyFontList;
    const QString iniFileKeyEditorFont;
};

#endif // MAINWINDOW_H
