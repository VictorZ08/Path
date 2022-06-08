#include "dataverification.h"

DataVerificationAep::DataVerificationAep(QObject *parent) : QObject(parent)
{
    qDebug()<<"Create DataVerification";
}

//-------------------------------------
DataVerificationAep::~DataVerificationAep()
{
   qDebug()<<"Kill DataVerification";
}

//-------------------------------------
void DataVerificationAep::checkingFileTxtAep(const QString &inNameFile)
{//Выполнить через исключение
    if(!openFile(inNameFile)) {
        qDebug() << "Message fun ERROR open";
        return;
    }
    //readFile(inNameFile);

}

//-------------------------------------
bool DataVerificationAep::openFile(const QString &inNameFile)
{
    QFile file(inNameFile);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qWarning("Cannot open file for reading");
        return 1;
    }
    QStringList line;
    QTextStream in(&file);
    while (!in.atEnd())
        line.append(in.readLine());

    file. close();
return 0;
}

//-------------------------------------
void DataVerificationAep::readFile(const QString &inNameFile)
{

}

bool DataVerificationAep::isDigitals(QStringView inStr)
{
    QString::const_iterator it = std::find(inStr.begin(), inStr.end(),
                                           std::not1(std::ptr_fun(::isdigit)));
    return it == inStr.end();
}

bool DataVerificationAep::isModes(const QString& inNameFiles)
{
    if(inNameFiles.contains("выкл", Qt::CaseInsensitive) ||
       inNameFiles.contains("вкл", Qt::CaseInsensitive))
    {
        return true;
    }
return false;
}

bool DataVerificationAep::isEvenFiles(const QFileInfoList& inFiles)
{
    return inFiles.count() % 2 == 0;
}

bool DataVerificationAep::isEmptyOneStr(QStringView inStr)
{

}

bool DataVerificationAep::compareResistance(QStringView inStr)
{

}
