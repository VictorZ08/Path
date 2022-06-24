#include "dataverification.h"

#include <fstream>
#include <QDebug>

#include <functional>
#include <algorithm>
#include <cctype>
#include <string>
#include <QRegularExpression>

constexpr int limitCountLineFile = 15;
constexpr int limitCountTab = 15;

DataVerificationAep::DataVerificationAep(const QFileInfoList& inFiles)
    : checkFiles{inFiles}
{
    qDebug()<<"Create DataVerificationAep";
}

QStringList DataVerificationAep::startCheckFiles()
{
    QStringList reportErrors;
    QPair<QStringList, QStringList> loadErrors;

    for(auto& file : checkFiles) {

        if(readFile(file.absoluteFilePath())) {
            loadErrors.first.append("Повреждённые файлы: ");
            loadErrors.second.append(file.absoluteFilePath());
            continue;
        }

        if(isDigitals(m_buffer)) {
            loadErrors.first.append("В файлах имеются буквы: ");
            loadErrors.second.append(file.absoluteFilePath());
        }

        if(isModes(file.absoluteFilePath())) {
            loadErrors.first.append("В имени файла отсутствует вкл/выкл: ");
            loadErrors.second.append(file.absoluteFilePath());
        }

        if(isCountLineFile(m_buffer)) {
            loadErrors.first.append("В файлах имеется превышение количества строк: ");
            loadErrors.second.append(file.absoluteFilePath());
        }

        if(compareResistance(m_buffer)) {
            loadErrors.first.append("Файлы имеют разное сопротивление: ");
            loadErrors.second.append(file.absoluteFilePath());
        }
    }

    if(isEvenFiles(checkFiles)) {
        loadErrors.first.append("Комплекты имеют не чётное количество файлов: ");
        loadErrors.second.append(checkFiles.at(0).absoluteFilePath());
    }
return reportErrors;
}

//-------------------------------------
bool DataVerificationAep::readFile(const QString& inPathFile) const
{
    QFile file(inPathFile);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)){
        return true;
    }

    QTextStream in(&file);
    while (!in.atEnd())
        m_buffer = in.readAll();

    file. close();
return false;
}

bool DataVerificationAep::isDigitals(const QString& inBuffer) const
{
    //QString str("\n.-.01-23\n45.67f89.\n");
    QRegularExpression re("^[\\s0-9.-]+$", QRegularExpression::CaseInsensitiveOption);
    QRegularExpressionMatch match = re.match(inBuffer);
    if (!match.hasMatch()) {
        return true;
    }
    return false;
}

bool DataVerificationAep::isModes(const QString& inNameFile) const
{
    if(inNameFile.contains("выкл", Qt::CaseInsensitive) ||
       inNameFile.contains("вкл", Qt::CaseInsensitive))
    {
        return true;
    }
return false;
}

bool DataVerificationAep::isEvenFiles(const QFileInfoList& inFiles) const
{
    return inFiles.count() % 2 == 0;
}

bool DataVerificationAep::isCountLineFile(const QString& inBuffer) const
{
    size_t line = inBuffer.count('\n');

    qDebug()<<"line: " << line;
    if(line == limitCountLineFile)
        return false;

    return true;
}

bool DataVerificationAep::isCountTab(const QString& inBuffer) const
{
    int res = inBuffer.count('\t');
    return res == limitCountTab ? false : true;
}

bool DataVerificationAep::compareResistance(QStringView inStr) const
{
    /*QString tempBuffOneStr = inBuff.left(inBuff.indexOf('\n', 1)).toString();

    QMap<QString, QString>::iterator itMap = m_mapStr.find(tempBuffOneStr);
    if(!m_mapStr.contains(tempBuffOneStr)) {
        m_mapStr.insert(tempBuffOneStr,
                        QString::number(Random::randOneStringAep(tempBuffOneStr.toDouble())));
        return m_mapStr.value(tempBuffOneStr);
    }
    else {
        tempBuffOneStr = m_mapStr.value(tempBuffOneStr);
        m_mapStr.erase(itMap);
        return tempBuffOneStr;
    }*/
}
