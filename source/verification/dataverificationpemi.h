#ifndef DATAVERIFICATIONPEMI_H
#define DATAVERIFICATIONPEMI_H

#include "xlsxdocument.h"
#include "QFileInfoList"

struct sCoordSerchTable;

class DataVerificationPemi
{
public:
    DataVerificationPemi();

    QStringList getLog();
    void checkFile(const QFileInfo& inFiles,
                   const sCoordSerchTable& inCoord);

protected:
    bool isTableCoordinates(const sCoordSerchTable& inCoord,
                            const QString&& nameCell,
                            const QXlsx::Document& xlsxFile);
    void isNoiseLessSignal(QXlsx::Document& xlsxFile,
                           const QString&& nameFile);
    void isEmptyCell(QXlsx::Document& xlsxFile,
                     const QString&& nameFile);
    void isDigitalsCell(QXlsx::Document &xlsxFile,
                        const QString&& nameFile);
    void isSpaceNameSheets(QXlsx::Document &xlsxFile,
                           const QString&& nameFile);

private:
    QMultiMap<QString, QString> m_loadErrors;

    DataVerificationPemi(const DataVerificationPemi& lhs) = delete;
    DataVerificationPemi& operator=(const DataVerificationPemi& lhs) = delete;
};

#endif // DATAVERIFICATIONPEMI_H
