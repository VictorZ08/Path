#ifndef SORTFILES_H
#define SORTFILES_H

#include "set.h"

/**
    @author Victor Zaytcev
*/
void sortFilesToComplectAep(pairFiFiL& inFiles);

void shuffleFiles(QFileInfoList& inFiles);

void shuffleFiles(QVector<pairFiFiL>& inFiles);
#endif // SORTFILES_H
