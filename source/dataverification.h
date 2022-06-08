#ifndef DATAVERIFICATION
#define DATAVERIFICATION

#include <QObject>
#include <QFileInfoList>
#include <QDebug>

class DataVerificationAep : public QObject
{
     Q_OBJECT
public:
    explicit DataVerificationAep(QObject *parent = nullptr);
    ~DataVerificationAep();

    bool isDigitals(QStringView inStr);
    bool isModes(const QString& inNameFiles);
    bool isEvenFiles(const QFileInfoList& inFiles);
    bool isEmptyOneStr();
    bool compareResistance();



    void checkingFileTxtAep(const QString &inNameFile);
    //void checkingFileExelPemi(const QFile &inNameFile);

signals:


private:
    bool openFile(const QString &inNameFile);
    void readFile(const QString &inNameFile);
    /*void checkSymbols(const QString &inNameFile);
    void checkDoubleSymbols(const QString &inNameFile);
    void checkSymbolsBeforePoint(const QString &inNameFile);
    void checkSymbolsAfterPoint(const QString &inNameFile);
    void checkEndLine(const QString &inNameFile);
    void checkParityFiles(const QString &inNameFile);*/

private:


};

#endif // DATAVERIFICATION

/*
 * АЭП:
 *1.Проверка на символы
 *2.Проверка на 00.00(2 символа после точки)
 *3.Проверка на .00 и 00. (символы до точки и после)
 *4.Проверка на конец строки после последнего значения
 *5.Проверка на четность файлов//
 *6.Проверка названия файлов на присутствие реимов ("выкл" и "вкл")//
 *7.Проверка одинаковых сопротивлений в 2х реж.//
 *
 *ПЭМИ:
 *
*/

