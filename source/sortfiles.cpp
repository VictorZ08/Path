#include "sortfiles.h"
#include <random>

/**
    @brief sortFilesToComplectAep
    Сортирует файлы в каталоге
    @param inSets ссылка на каталог с файлами
*/
void sortFilesToComplectAep(Set& inSets)
{
    for(auto& set : inSets.getSetsAep()) {
        std::sort(set.second.begin(), set.second.end(), [](QFileInfo& a, QFileInfo& b) {
            return a.baseName().contains("выкл",
                                         Qt::CaseInsensitive); });
    }
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
