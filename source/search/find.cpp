#include "find.h"
#include "xlsxdocument.h"

#include <QDir>
#include <QDebug>

Find::Find(QObject* parent)
    : QObject(parent)
{
    qDebug()<<"Create Find";
}

Find::~Find()
{
    qDebug()<<"~Find";
}

//-------------------------------------
void Find::slotThreadFind()
{
    foreach(auto folder, m_urlFoldersOrder) {
        findFilesInOrder(folder);
        insertFilesInHash();
        emit tempStart();
    }
    emit threadFinished();
}

//-------------------------------------
void Find::findFilesInOrder(const QString& inNameOrder)
{
    QDir dir(inNameOrder);
    QFileInfoList listFolders = dir.entryInfoList( QDir::Dirs | QDir::NoDotAndDotDot );

    dir.setFilter(QDir::Files);
    dir.setNameFilters( QStringList() << "*.xlsx" );
    QFileInfoList listExelFiles = dir.entryInfoList();
    foreach(auto file, listExelFiles)
        m_listFileExel.append(file.absoluteFilePath());

    foreach(auto folder, listFolders) {
        try {
            findFilesExelInPositions(folder);
        }  catch (QStringList &) {
            qDebug()<<"throw!";
        }
    }
}

//-------------------------------------
QStringList& Find::findFilesExelInPositions(const QFileInfo& inFolder)
{
    /*Рекурсивная ф-ия, ищет файлы в каталогах*/
    QDir dir(inFolder.absoluteFilePath());
    if ( !dir.exists() )
        qFatal( "No such directory: Error findFilesExelInPositions()" );

    QStringList dirList = dir.entryList( QDir::Dirs | QDir::NoDotAndDotDot );

    dir.setFilter(QDir::Files);
    dir.setNameFilters( QStringList() << "*.xlsx" );
    QFileInfoList fileList = dir.entryInfoList();

    QFileInfoList::Iterator it_fil = fileList.begin();
    for ( ; it_fil != fileList.end(); ++it_fil ) {
        m_listFileExel.append(it_fil->absoluteFilePath());
        throw m_listFileExel;
    }
    QStringList::Iterator it_dir = dirList.begin();
    for ( ; it_dir != dirList.end(); ++it_dir ) {
        QDir curDir = dir;
        curDir.cd( *it_dir );
        QStringList curList = findFilesExelInPositions( curDir.absolutePath() );
    }
return m_listFileExel;
}

//-------------------------------------
void Find::insertFilesInHash()
{
    m_hashExelFiles.reserve(m_listFileExel.count());
    foreach(auto fileExel, m_listFileExel) {
        QXlsx::Document xlsx(fileExel);
        QXlsx::Cell *cell;
        QString     val;
        uint col = 1;
        while((cell = xlsx.cellAt(1, col++)))
            val.append(cell->value().toString().toLower());

        m_hashExelFiles.insert(fileExel, val);
    }
}

//-------------------------------------
void Find::setUrlFolders(const QStringList& inUrlOrder)
{
    m_urlFoldersOrder = inUrlOrder;
}
