#include "converter.h"
#include <QProcess>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QStringList>
#include <iostream>

#define FONT_ID_DEFAULT 0

#define PLATEX_TIMEOUT_INTERVAL 500
#define PLATEX_MAX_TRY_COUNT 40

#define PROCESS_TIMEOUT 10000

converter::converter(QObject *parent, QComboBox *comboBoxFont) :
    QObject(parent),
    fontList(0),
    magnitude(1),
    font(FONT_ID_DEFAULT),
    texEqSource(""),
    fileName("temp"),
    type("png"),
    pathPlatex(""), pathDvips(""), pathDvipng(""), pathImageMagick(""),
    platex(0), dvips(0), dvipng(0), imageMagickConvert(0),
    conversionSuccessed(false),
    preamble(""),
    convMode(prev)
{
    this->platex = new QProcess(this);
    this->dvips  = new QProcess(this);
    this->dvipng = new QProcess(this);
    this->imageMagickConvert = new QProcess(this);


    fontList.clear();
    for(int i = 0; i < comboBoxFont->count(); i++) {
        fontList.push_back(comboBoxFont->itemText(i));
    }
}

converter::~converter()
{
    delete platex; platex = 0;
    delete dvips; dvips = 0;
    delete imageMagickConvert; imageMagickConvert = 0;
}

bool converter::setup(const QString &texEqSource, const int magnitude, const int font,
                      const QString &fileName, const QString &type,
                      const QStringList &packageList, const QStringList &includeList,
                      const mode md)
{
    if(this->pathPlatex == "") return false;
    if(this->pathDvips  == "") return false;
    if(this->type == "png") {
        if(this->pathDvipng == "") return false;
    } else {
        if(this->pathImageMagick == "") return false;
    }

    this->convMode = md;

    this->texEqSource = texEqSource;
    this->texEqSource.replace('\n', ' ');
    this->magnitude = magnitude;
    this->font = font;
    this->fileName = fileName != tr("") ? fileName : tr("temp");
    this->type = type;

    this->preamble.clear();
    for (int i = 0; i < packageList.size(); i++) {
        this->preamble += tr("\\usepackage{%1}\n").arg(packageList.at(i));
    }
    for (int i = 0; i < includeList.size(); i++) {
        this->preamble += tr("\\input{%1}\n").arg(includeList.at(i));
    }

    return true;
}

void converter::execute()
{
    this->conversionSuccessed = convert();/*(texEqSource, magnitude, font, fileName, type);*/
    emit convFinished();
}

bool converter::convert()
{
    if(!createTexFile()) return false;
    if(!createDviFile()) return false;

    if (this->convMode == converter::conv) {
        if(!createEpsFile()) return false;
    }

    return createPngFile();
}

bool converter::createTexFile()
{
    QFile texFile(tr("temp.tex"));
    texFile.open(QIODevice::WriteOnly);
    QTextStream out(&texFile);

    out << tr("\\documentclass{jarticle}") << endl;
    if(this->font != FONT_ID_DEFAULT) {
        out << tr("\\usepackage{%1}").arg(fontList[this->font]) << endl;
    }
    out << this->preamble << endl;
    out << tr("\\begin{document}") << endl;
    out << tr("\\thispagestyle{empty}") << endl;
    out << tr("\\begin{eqnarray*}") << endl;
    out << this->texEqSource << endl;
    out << tr("\\end{eqnarray*}") << endl;
    out << tr("\\end{document}") << endl;

    texFile.close();
    return true;
}

bool converter::createDviFile()
{
    //std::cout << "entered 'createDviFile'" << std::endl;
    if(!processExecutable(this->platex)) {
        //std::cout << "platex is not executable" << std::endl;
        return false;
    }

    this->platex->start(tr("%1/platex temp.tex").arg(this->pathPlatex));

    bool successed = true;
    int loopCounter = 0;

    QString platexStdOut;
    while(1){
        bool finished = this->platex->waitForFinished(PLATEX_TIMEOUT_INTERVAL);
        platexStdOut += this->platex->readAllStandardOutput();
        if(finished) {
            break;
        } else {
            if(platexStdOut.contains(tr("? ")) ||
               platexStdOut.contains(tr("Enter file name: ")))
            {
                this->platex->write(tr("X\n").toLocal8Bit());
                //std::cout << "platex finished with some error" << std::endl;
                successed = false;
            }
        }

        if(++loopCounter > PLATEX_MAX_TRY_COUNT) {
            successed = false;
            this->platex->terminate();
            break;
        }
    }

    //if(successed) std::cout << "platex successfully finished" << std::endl;
    return successed;
}

bool converter::createEpsFile()
{
    if(!processExecutable(this->dvips)) return false;

    dvips->start(tr("%1/dvips -E -Ppdf -x %2 temp.dvi -o %3.eps")
                 .arg(this->pathDvips)
                 .arg(this->magnitude * 1000)
                 .arg(this->fileName));
    return dvips->waitForFinished(PROCESS_TIMEOUT);
}

bool converter::createPngFile()
{
    if (this->type == "png") {

        if(!processExecutable(this->dvipng)) return false;

        this->dvipng->start(tr("%1/dvipng temp.dvi -T tight -D %2 -bg Transparent -o %3.png")
                            .arg(this->pathDvipng)
                            .arg(this->magnitude * 100)
                            .arg(this->fileName));

        return this->dvipng->waitForFinished(PROCESS_TIMEOUT);

    } else {

        if(!processExecutable(this->imageMagickConvert)) return false;

        this->imageMagickConvert->start(tr("%1/convert %2.eps %2.%3")
                                  .arg(this->pathImageMagick)
                                  .arg(this->fileName).arg(this->type));

        return this->imageMagickConvert->waitForFinished(PROCESS_TIMEOUT);

    }
}

bool converter::processExecutable(QProcess *process)
{
    if(process->state() != QProcess::NotRunning) {
        return false;
    } else {
        return true;
    }
}

void converter::subscribePathPlatex(const QString &pathPlatex)
{
    this->pathPlatex = pathPlatex;
}

void converter::subscribePathDvips(const QString &pathDvips)
{
    this->pathDvips = pathDvips;

    subscribePathDvipng(pathDvips);
}

void converter::subscribePathDvipng(const QString &pathDvipng)
{
    this->pathDvipng = pathDvipng;
}

void converter::subscribePathImageMagick(const QString &pathImageMagick)
{
    this->pathImageMagick = pathImageMagick;
}

QString converter::getPathPlatex() const
{
    return this->pathPlatex;
}

QString converter::getPathDvips() const
{
    return this->pathDvips;
}

QString converter::getPathImageMagick() const
{
    return this->pathImageMagick;
}
