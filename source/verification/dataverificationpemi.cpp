#include "dataverificationpemi.h"
#include "generator/generatorpemiwidget.h"

#include <QRegularExpression>
#include "QDebug"

DataVerificationPemi::DataVerificationPemi()
{
    qDebug()<<"Create DataVerificationPemi";
}

QStringList DataVerificationPemi::getLog()
{
    QStringList reportErrors;
    if(m_loadErrors.isEmpty()){
        return reportErrors;
    }

    QMultiMap<QString, QString>::iterator itLeft = m_loadErrors.begin();
    QMultiMap<QString, QString>::iterator itRight = m_loadErrors.begin() + 1;
    reportErrors.append(itLeft.key());
    reportErrors.append(itLeft.value());
    for( ; itRight != m_loadErrors.end(); ++itRight) {
        if(itLeft.key() == itRight.key()) {
            reportErrors.append(itRight.value());
        }
        else {
            itLeft = itRight;
            reportErrors.append("\n");
            reportErrors.append(itLeft.key());
            reportErrors.append(itLeft.value());
        }
    }
    return reportErrors;
}

void DataVerificationPemi::checkFile(const QFileInfo& inFile,
                                     const sCoordSerchTable& inCoord)
{
        QXlsx::Document xlsxOpenFile(inFile.absoluteFilePath());
        if(isTableCoordinates(inCoord,
                              "Комплект",
                              xlsxOpenFile)) {
            m_loadErrors.insert("В файлах титульная рамка за диапазоном: ",
                              inFile.absoluteFilePath());
        }

        isNoiseLessSignal(xlsxOpenFile,
                          inFile.absoluteFilePath());

        isEmptyCell(xlsxOpenFile,
                    inFile.absoluteFilePath());

        isDigitalsCell(xlsxOpenFile,
                       inFile.absoluteFilePath());

        isSpaceNameSheets(xlsxOpenFile,
                          inFile.absoluteFilePath());

}

bool DataVerificationPemi::isTableCoordinates(const sCoordSerchTable& inCoord,
                                              const QString&& nameCell,
                                              const QXlsx::Document& xlsxFile)
{
    QXlsx::Cell* cell;
    QString val;
    for(int row = inCoord.rowStart; row < inCoord.rowEnd; ++row) {
        for(int col = inCoord.columnStart; col < inCoord.columnEnd; ++col) {
            cell = xlsxFile.cellAt(row, col);
            if(cell == nullptr)
               continue;
            
            val.append(cell->value().toString());
            if(val.contains(nameCell, Qt::CaseInsensitive)) {               
               return false;
            }
         }
    }
    return true;
}

void DataVerificationPemi::isNoiseLessSignal(QXlsx::Document& xlsxFile,
                                             const QString&& nameFile)
{
    QStringList sheetNames = xlsxFile.sheetNames();
    for(auto& sheet : sheetNames) {
        if(sheet == "Описание")
            continue;
                
        xlsxFile.selectSheet(sheet);
        int row = 1, col = 2;
        QVariant valFrequency = xlsxFile.read(row, 1);
        while(valFrequency.isValid()) {
            QVariant valSignal = xlsxFile.read(row, col);
            QVariant valNoise = xlsxFile.read(row, ++col);
            if(!valNoise.isValid() || !valSignal.isValid()) {
                ++row, col = 2;
                valFrequency = xlsxFile.read(row, 1);
                continue;
            }
            if(valNoise.toDouble() >= valSignal.toDouble()) {
                m_loadErrors.insert("Сигнал меньше или равен шуму: ",
                                    nameFile + " - " + sheet + " - " + valFrequency.toString() + " MHz");
                ++row, col = 2;
                continue;
            }
            ++row, col = 2;
            valFrequency = xlsxFile.read(row, 1);
        }
    }
}

void DataVerificationPemi::isEmptyCell(QXlsx::Document &xlsxFile,
                                       const QString&& nameFile)
{
    QStringList sheetNames = xlsxFile.sheetNames();
    for(auto& sheet : sheetNames) {
        if(sheet == "Описание")
            continue;

        xlsxFile.selectSheet(sheet);
        int row = 1, col = 2;
        QVariant valFrequency = xlsxFile.read(row, 1);
        if(!valFrequency.isValid()) {
            m_loadErrors.insert("Пустой лист: ",
                                nameFile + " - " + sheet);
        }
        while(valFrequency.isValid()) {
            QVariant valSignal = xlsxFile.read(row, col);
            QVariant valNoise = xlsxFile.read(row, ++col);            
            if(!valNoise.isValid() && !valSignal.isValid()) {
                ++row, col = 2;
                valFrequency = xlsxFile.read(row, 1);
                continue;
            }
            if(!valNoise.isValid() || !valSignal.isValid()) {
                m_loadErrors.insert("Частота содержит пустую ячейку: ",
                                    nameFile + " - " + sheet + " - " + valFrequency.toString() + " MHz");
                ++row, col = 2;
                continue;
            }
            ++row, col = 2;
            valFrequency = xlsxFile.read(row, 1);
        }
    }
}

void DataVerificationPemi::isDigitalsCell(QXlsx::Document &xlsxFile,
                                          const QString&& nameFile)
{
    auto sheetNames = xlsxFile.sheetNames();
    for(auto& sheet : sheetNames) {
        if(sheet == "Описание")
            continue;

        int row = 1;
        xlsxFile.selectSheet(sheet);
        QVector<QVariant> vals;
        QVariant valFrequency = xlsxFile.read(row, 1);

        QRegularExpression regOneStr("^[\\sа-яА-ЯёЁa-zA-Z]+$",
                              QRegularExpression::CaseInsensitiveOption);
        auto match = regOneStr.match( valFrequency.toString() );
        if (match.hasMatch()) {
            continue;
        }

        QRegularExpression regVal("^[\\s0-9.-]+$",
                              QRegularExpression::CaseInsensitiveOption);
        while(valFrequency.isValid()) {
            vals.clear();
            for(int col = 1; col < 4; ++col) {
                vals.push_back(xlsxFile.read(row, col));
            }
            for(const auto& val : vals) {
                auto match = regVal.match( val.toString() );
                if(!val.isValid()) {
                    continue;
                }
                if (!match.hasMatch()) {
                    m_loadErrors.insert("Значения имеют некорректный символ: ",
                                        nameFile + " - " + sheet + " - " + valFrequency.toString() + " MHz");
                }
            }
            ++row;
            valFrequency = xlsxFile.read(row, 1);
        }
    }
}

void DataVerificationPemi::isSpaceNameSheets(QXlsx::Document &xlsxFile,
                                             const QString&& nameFile)
{
    auto sheetNames = xlsxFile.sheetNames();
    for(auto& sheet : sheetNames) {
        if(sheet.contains(" ")) {
            m_loadErrors.insert("Названия листов имеют пробелы: ",
                                nameFile + " - " + sheet);
        }
    }
}

