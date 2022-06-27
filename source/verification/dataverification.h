#ifndef DATAVERIFICATION
#define DATAVERIFICATION

#include <QObject>
#include <QFileInfoList>
#include <QDebug>

using  pairSLSL = QPair<QStringList, QStringList>;

class DataVerificationAep
{   
public:
    DataVerificationAep(const QFileInfoList& inFiles);
    QStringList startCheckFiles();

    DataVerificationAep (const DataVerificationAep& lhs) = delete;
    DataVerificationAep& operator=(const DataVerificationAep& lhs) = delete;

protected:
    bool isDigitals(const QString& inBuffer) const;
    bool isModes(const QString& inNameFile) const;
    bool isEvenFiles(const QFileInfoList& inFiles) const;
    bool isCountLineFile(const QString& inBuffer) const;
    bool isCountTab(const QString& inBuffer) const;
    bool compareResistance(QStringView inStr) const;

private:
    bool readFile(const QString& inPathFile) const;

private:
    QFileInfoList m_checkFiles;
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

