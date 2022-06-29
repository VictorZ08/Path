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

QStringList DataVerificationAep::getData()
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

void DataVerificationAep::checkFiles(const QFileInfoList &inFiles)
{
    for(auto& file : inFiles) {

        if(readFile(file.absoluteFilePath())) {
            m_loadErrors.insert("Повреждённые файлы: ",
                              file.absoluteFilePath());
            continue;
        }

        if(isDigitals(m_buffer)) {
            m_loadErrors.insert("В файлах имеются буквы: ",
                              file.absoluteFilePath());
        }

        if(isModes(file.absoluteFilePath())) {
            m_loadErrors.insert("В имени файла отсутствует вкл/выкл: ",
                              file.absoluteFilePath());
        }

        if(isCountLineFile(m_buffer)) {
            m_loadErrors.insert("В файлах имеется превышение количества строк: ",
                              file.absoluteFilePath());
        }

        if(isCountTab(m_buffer)) {
            m_loadErrors.insert("В файлах имеется превышение количества табуляции: ",
                              file.absoluteFilePath());
        }

        if(compareResistance(m_buffer)) {
            m_loadErrors.insert("Файлы имеют разное сопротивление: ",
                              file.absoluteFilePath());
        }
    }

    if(isEvenFiles(inFiles)) {
        m_loadErrors.insert("Комплекты имеют не чётное количество файлов: ",
                          inFiles.at(0).absoluteFilePath());
    }
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
    QRegularExpression re("^[\\s0-9.-]+$",
                          QRegularExpression::CaseInsensitiveOption);
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
    return false;
}
