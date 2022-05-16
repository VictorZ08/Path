#include "database.h"

#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlError>
#include <QDateTime>
#include <QDir>
#include <QDebug>

DataBase::DataBase(QObject *parent) : QObject(parent)
{
    //if (!createConnectDB()){}
       //return EXIT_FAILURE;
    qDebug() << "Create DataBase";
}

DataBase::~DataBase()
{
   qDebug() << "~DataBase";
}

QString DataBase::getDateUpdateServerDB()
{
    QSqlQuery query;
    if(!query.exec("SELECT dateUpdateServer FROM tableUpdateServer")) {
        qDebug() << "Unable to execute getDateUpdateServerDB";
        return m_updateDateServer;
    }
    int numberColumn = query.record().indexOf("dateUpdateServer");
    QString result;
    while (query.next())
        result = query.value(numberColumn).toString();

    return result;
}

quint64 DataBase::countFilesDB()
{
    QSqlQuery query("SELECT COUNT(pathFiles) FROM tableExelFies");
    if (query.next())
        return m_countRows = query.value(0).toInt();
    return 0;
}

void DataBase::createServerDB()
{
    QSqlQuery query;
    if(!query.exec("create table tableExelFies ("
                   "pathFiles varchar(20), "
                   "oneStringInFile varchar(20))")) {
        qDebug() << "Unable table tableExelFies";
        return;
    }

    query.prepare("INSERT INTO tableExelFies (pathFiles, oneStringInFile)"
                      "VALUES(:first, :last);");
    QHash<QString, QString>::const_iterator i = m_hash->constBegin();
    for(; i != m_hash->constEnd(); ++i) {
        query.bindValue(":first", i.key());
        query.bindValue(":last",  i.value());
        query.exec();
    }
}

void DataBase::createDateServerDB()
{
    QSqlQuery query;
    if(!query.exec("create table tableUpdateServer ("
                     "dateUpdateServer varchar(20))")) {
       qDebug() << "Unable table tableUpdateServer";
       return;
    }

    query.prepare("INSERT INTO tableUpdateServer (dateUpdateServer)"
                      "VALUES(:date);");
    m_updateDateServer = QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm");
    query.bindValue(":date", m_updateDateServer);
    query.exec();
}

void DataBase::outInHashTableDB()
{
    QSqlQuery query;
    if(!query.exec("SELECT pathFiles FROM tableExelFies")) {
        qDebug() << "Unable to execute query";
        return;
    }
    int fieldNo = query.record().indexOf("pathFiles");
    while (query.next())
        m_dataInColumnDB.append(query.value(fieldNo).toString());
}

void DataBase::compareDB()
{
    QStringList::const_iterator i = m_dataInColumnDB.begin();
    for(; i != m_dataInColumnDB.end(); ++i)
        m_hash->remove(*i);
}

void DataBase::insertDataInDB()
{
    QSqlQuery query;
    query.prepare("INSERT INTO tableExelFies (pathFiles, oneStringInFile)"
                      "VALUES(:first, :last);");
    QHash<QString, QString>::const_iterator i = m_hash->constBegin();
    for(; i != m_hash->constEnd(); ++i) {
        query.bindValue(":first", i.key());
        query.bindValue(":last",  i.value());
        query.exec();
    }
}

void DataBase::updateDateServerDB()
{
    QSqlQuery query;
    query.prepare("UPDATE tableUpdateServer SET dateUpdateServer = :date");
    m_updateDateServer = QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm");
    query.bindValue(":date", m_updateDateServer);
    query.exec();
}

bool DataBase::createConnectDB()
{
    QSqlDatabase db;
    pathDB = QDir::currentPath() + "/db/db.sql";
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(pathDB);
    if (!db.open()) {
        qDebug() << "Cannot open database:" << db.lastError().text();
        return false;
    }
    return true;
}

void DataBase::slotThreadDB()
{
    if (!createConnectDB()){
        qDebug() << "No createConnectDB!!!";
        return;
    }
    if(countFilesDB() == false) {
        createServerDB();
        createDateServerDB();
        emit threadFinished();
        return;
    }
    outInHashTableDB();
    compareDB();
    insertDataInDB();
    updateDateServerDB();
    emit threadFinished();
}

void DataBase::disconnectDB()
{
    QString qs;
        {
            qs.append(QSqlDatabase::database().connectionName());
        }
        QSqlDatabase::removeDatabase(qs);
        qDebug() << "AfterDelete" << QSqlDatabase::database().connectionNames();
}
