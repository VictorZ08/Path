#ifndef FIND_H
#define FIND_H

#include <QObject>
#include <QHash>

class QFileInfo;

class Find : public QObject
{
    Q_OBJECT
public:
    explicit Find(QObject* parent = nullptr);
    ~Find();

    void setUrlFolders(const QStringList& inUrlOrder);

    QHash<QString, QString>& getHashExelFiles()
                                    { return  m_hashExelFiles;}

private:
    void findFilesInOrder(const QString& inNameOrder);
    void insertFilesInHash();

    QStringList& findFilesExelInPositions(const QFileInfo& inFolder);

public slots:
    void slotThreadFind();

signals:
    void tempStart();
    void threadFinished();

private:
    QStringList m_urlFoldersOrder;
    QHash<QString, QString> m_hashExelFiles;
    QStringList     m_listFileExel;

};

#endif // FIND_H
