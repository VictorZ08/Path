#include "dataverification.h"

DataVerification::DataVerification(QObject *parent) : QObject(parent)
{
    qDebug()<<"Create DataVerification";
}

//-------------------------------------
DataVerification::~DataVerification()
{
   qDebug()<<"Kill DataVerification";
}

//-------------------------------------
void DataVerification::checkingFileTxtAep(const QString &inNameFile)
{//Выполнить через исключение
    if(!openFile(inNameFile)) {
        qDebug() << "Message fun ERROR open";
        return;
    }
    //readFile(inNameFile);

}

//-------------------------------------
bool DataVerification::openFile(const QString &inNameFile)
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
void DataVerification::readFile(const QString &inNameFile)
{

}
