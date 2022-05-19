#include "sortfiles.h"
#include <random>

/**
    @brief sortFilesToComplectAep
    Сортирует файлы в каталоге
    @param inFiles ссылка на каталог с файлами
*/
void sortFilesToComplectAep(pairFiFiL& inFiles)
{
    QFileInfoList::iterator itBeg = inFiles.second.begin();
    QFileInfoList::iterator itEnd = inFiles.second.end();

    std::sort(itBeg, itEnd, [](QFileInfo& a, QFileInfo& b) {
        return a.baseName().contains("выкл",
                                     Qt::CaseInsensitive); });
}

/**
    @brief shuffleExelFilesPemi
    Перетасовывает файлы
    @param inFiles ссылка на файлы
*/
void shuffleFiles(QFileInfoList& inFiles)
{
    std::random_device rd;
    std::shuffle(inFiles.begin(), inFiles.end(), rd);
}
void shuffleFiles(QVector<pairFiFiL>& inFiles)
{
    std::random_device rd;
    std::shuffle(inFiles.begin(), inFiles.end(), rd);
}
