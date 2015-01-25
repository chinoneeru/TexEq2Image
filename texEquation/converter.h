#ifndef CONVERTER_H
#define CONVERTER_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QComboBox>

class converter : public QObject {
    Q_OBJECT

public:
    converter(QObject *parent = 0, QComboBox *comboBoxFont = 0);
    ~converter();
    bool isSuccessed() { return conversionSuccessed; }
    bool setup(const QString &texEqSource, const int magnitude, const int font,
                 const QString &fileName, const QString &type,
               const QStringList &packageList, const QStringList &includeList);

public slots:
    void execute();
    void subscribePathPlatex(const QString &pathPlatex);
    void subscribePathDvips(const QString &pathDvips);
    void subscribePathImageMagick(const QString &pathImageMagick);
    QString getPathPlatex() const;
    QString getPathDvips() const;
    QString getPathImageMagick() const;

signals:
    void convFinished(void);

private:
    /*bool convert(const QString &texEqSource, const int magnitude, const int font,
                 const QString &fileName, const QString &type);*/
    bool convert();
    bool createTexFile();
    bool createDviFile();
    bool createEpsFile();
    bool createPngFile();
    bool processExecutable(class QProcess *process);

private:
    QStringList fontList;
    int magnitude, font;
    QString texEqSource, fileName, type;
    QString pathPlatex, pathDvips, pathImageMagick;
    class QProcess *platex;
    class QProcess *dvips;
    class QProcess *imageMagickConvert;
    bool conversionSuccessed;
    QString preamble;
};

#endif // CONVERTER_H
