#include "dataverification.h"

void DataVerificationAep::checkFiles(const QFileInfoList& inFiles)
{
    for(auto& file : inFiles) {
        if(readFile(file.absoluteFilePath())) {
            //запись в лог open error nameFile
            continue;
        }

        /*if() {

        }

        if() {

        }

        if() {

        }

        if() {

        }

        if() {

        }

        if() {

        }*/

    }
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

bool DataVerificationAep::isDigitals(QStringView inStr) const
{
    QString::const_iterator it = std::find(inStr.begin(), inStr.end(),
                                           std::not1(std::ptr_fun(::isdigit)));
    return it == inStr.end();
}

bool DataVerificationAep::isModes(const QString& inNameFiles) const
{
    if(inNameFiles.contains("выкл", Qt::CaseInsensitive) ||
       inNameFiles.contains("вкл", Qt::CaseInsensitive))
    {
        return true;
    }
return false;
}

bool DataVerificationAep::isEvenFiles(const QFileInfoList& inFiles) const
{
    return inFiles.count() % 2 == 0;
}

bool DataVerificationAep::isEmptyFile(QStringView inStr) const
{
    QString str(inStr.left(inStr.indexOf('\n', 1)).toString());
    return str.isEmpty();
}

bool DataVerificationAep::compareResistance(const QStringList& inStr) const
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
