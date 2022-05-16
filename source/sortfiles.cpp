#include "sortfiles.h"
#include <random>

/**
    @brief swapFilesToComplectAep
    Меняет местами файлы в каталоге
    @param inFiles ссылка на каталог с файлами
*/
void swapFilesToComplectAep(pairFiFiL& inFiles)
{
    int count = inFiles.second.count();
    int backRow = 0;
    for(int frontRow = 0; frontRow < count; frontRow++) {
        if(strstr(inFiles.second.at(frontRow).baseName().toUtf8(), "выкл")) {
            inFiles.second.swapItemsAt(frontRow, backRow++);
        }
    }
}

/**
    @brief shuffleExelFilesPemi
    Перетасовывает файлы
    @param inFiles ссылка на файлы
*/
void shuffleExelFilesPemi(QFileInfoList& inFiles)
{
    std::random_device rd;
    std::shuffle(inFiles.begin(), inFiles.end(), rd);
}

