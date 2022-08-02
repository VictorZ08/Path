#ifndef DATAVERIFICATIONAEP_H
#define DATAVERIFICATIONAEP_H

#include <QFileInfoList>
#include <QMultiMap>

using  pairSLSL = QPair<QStringList, QStringList>;

class DataVerificationAep
{   
public:
    DataVerificationAep();

    QStringList getLog();
    void checkFiles(const QFileInfoList &inFiles);

protected:
    bool isDigitals(const QString& inBuffer) const;
    bool isModes(const QString& inNameFile) const;
    bool isEvenFiles(const QFileInfoList& inFiles) const;
    bool isCountLineFile(const QString& inBuffer) const;
    bool isCountTab(const QString& inBuffer) const;
    bool isCompareResistance();

    void fetchOneString(QStringView inStr);

private:
    bool readFile(const QString& inPathFile) const;

private:
    QMultiMap<QString, QString> m_loadErrors;
    QList<QString> m_resistance;

    mutable QString m_buffer;

    DataVerificationAep (const DataVerificationAep& lhs) = delete;
    DataVerificationAep& operator=(const DataVerificationAep& lhs) = delete;
};

#endif // DATAVERIFICATION

