#include "dataverification.h"

#include <fstream>
#include <QDebug>

constexpr int limitCountLineFile = 15;
//const QString kLogLevelDebug = "Не цифровой символ";
//const QString kLogLevelDebug = "Не цифровой символ";

void DataVerificationAep::checkFiles(const QFileInfoList& inFiles)
{
    QPair<QStringList, QStringList> checkError;

    for(auto& file : inFiles) {

        if(readFile(file.absoluteFilePath())) {
            checkError.first.append("Файл не открывается!");
            checkError.second.append(file.absoluteFilePath());
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
    /*QString::const_iterator it = std::find(inStr.begin(), inStr.end(),
                                           std::not1(std::ptr_fun(::isdigit)));
    return it == inStr.end();*/
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

bool DataVerificationAep::isCountLineFile(const QString& inNameFile) const
{
    std::ifstream  in("C:/1.txt");
    if(!in.is_open()){
       qDebug()<<"no open";
    }
    size_t line = std::count(std::istreambuf_iterator<char>(in),
                 std::istreambuf_iterator<char>(), '\n');

    qDebug()<<"line: " << line;
    if(line < limitCountLineFile)
        return false;

return true;
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
