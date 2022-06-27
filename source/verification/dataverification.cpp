#include "dataverification.h"

#include <fstream>
#include <QDebug>

#include <functional>
#include <algorithm>
#include <cctype>
#include <string>
#include <QRegularExpression>

constexpr int limitCountLineFile = 35;
constexpr int limitCountTab = 102;

DataVerificationAep::DataVerificationAep()
{
    qDebug()<<"Create DataVerificationAep";
}

void DataVerificationAep::pushData(const QFileInfoList &inFiles)
{
    m_checkFiles.clear();
    m_checkFiles = inFiles;
}

QStringList DataVerificationAep::checkFiles()
{
    QStringList reportErrors;
    QVector<QPair<QString, QStringList>> loadErrors;
    loadErrors.reserve(7);

    for(auto& file : m_checkFiles) {

        if(readFile(file.absoluteFilePath())) {
            loadErrors[0].first.append("Повреждённые файлы: ");
            loadErrors[0].second.append(file.absoluteFilePath());
            continue;
        }

        if(isDigitals(m_buffer)) {
            loadErrors[1].first.append("В файлах имеются буквы: ");
            loadErrors[1].second.append(file.absoluteFilePath());
        }

        if(isModes(file.absoluteFilePath())) {
            loadErrors[2].first.append("В имени файла отсутствует вкл/выкл: ");
            loadErrors[2].second.append(file.absoluteFilePath());
        }

        if(isCountLineFile(m_buffer)) {
            loadErrors[3].first.append("В файлах имеется превышение количества строк: ");
            loadErrors[3].second.append(file.absoluteFilePath());
        }

        if(isCountTab(m_buffer)) {
            loadErrors[4].first.append("В файлах имеется превышение количества табуляции: ");
            loadErrors[4].second.append(file.absoluteFilePath());
        }

        if(compareResistance(m_buffer)) {
            loadErrors[5].first.append("Файлы имеют разное сопротивление: ");
            loadErrors[5].second.append(file.absoluteFilePath());
        }
    }

    if(isEvenFiles(m_checkFiles)) {
        loadErrors[6].first.append("Комплекты имеют не чётное количество файлов: ");
        loadErrors[6].second.append(m_checkFiles.at(0).absoluteFilePath());
    }
return reportErrors;
}

//-------------------------------------
bool DataVerificationAep::readFile(const QString& inPathFile) const
{
    QFile file(inPathFile);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
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
        return false;
    }
return true;
}

bool DataVerificationAep::isEvenFiles(const QFileInfoList& inFiles) const
{
    int a = inFiles.count() % 2;
    return inFiles.count() % 2 == 0 ? false : true;
}

bool DataVerificationAep::isCountLineFile(const QString& inBuffer) const
{
    size_t line = inBuffer.count('\n');
    return line == limitCountLineFile ? false : true;
}

bool DataVerificationAep::isCountTab(const QString& inBuffer) const
{
    size_t res = inBuffer.count('\t');
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
