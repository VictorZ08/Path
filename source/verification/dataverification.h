#ifndef DATAVERIFICATION
#define DATAVERIFICATION

#include <QObject>
#include <QFileInfoList>
#include <QDebug>

using  pairSLSL = QPair<QStringList, QStringList>;

class DataVerificationAep
{   
public:
    DataVerificationAep();

    QStringList getData();
    void checkFiles(const QFileInfoList &inFiles);

    DataVerificationAep (const DataVerificationAep& lhs) = delete;
    DataVerificationAep& operator=(const DataVerificationAep& lhs) = delete;

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
};

#endif // DATAVERIFICATION

