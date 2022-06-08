#ifndef DATAVERIFICATION
#define DATAVERIFICATION

#include <QObject>
#include <QFileInfoList>
#include <QDebug>

class DataVerificationAep
{
     Q_OBJECT
public:
    void checkFiles(const QFileInfoList& inFiles);
    DataVerificationAep() = delete;

protected:
    bool isDigitals(QStringView inStr) const;
    bool isModes(const QString& inNameFiles) const;
    bool isEvenFiles(const QFileInfoList& inFiles) const;
    bool isEmptyFile(QStringView inStr) const;
    bool compareResistance(const QStringList& inStr) const;

private:
    //void loadFile(const QString &fileName);////
    //bool openFile(const QString &inNameFile);
    bool readFile(const QString& inPathFile) const;

private:
    mutable QString m_buffer;
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

