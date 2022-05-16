#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>

class DataBase : public QObject
{
     Q_OBJECT
public:
    explicit DataBase(QObject *parent = nullptr);
    ~DataBase();

    void    setHash(QHash<QString, QString> &inHash) {m_hash = &inHash;}
    void    disconnectDB();

    bool    createConnectDB();
    quint64 countFilesDB();
    QString getDateUpdateServerDB();

public slots:
    void slotThreadDB();

signals:
    void tempStart();
    void threadFinished();

private:
    void createServerDB();
    void createDateServerDB();
    void updateDateServerDB();
    void outInHashTableDB();
    void compareDB();
    void insertDataInDB();

private:
    QString pathDB;
    QHash<QString, QString> *m_hash;
    QStringList m_dataInColumnDB;
    QString m_updateDateServer = "--:--";
    quint64 m_countRows = 0;


};

#endif // DATABASE_H
