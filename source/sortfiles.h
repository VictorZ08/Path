#ifndef SORTFILES_H
#define SORTFILES_H

#include "set.h"
class Set;
//class QFileInfoList;

/**
    @author Victor Zaytcev
*/
void sortFilesToComplectAep(Set& inFiles);

void shuffleFiles(QFileInfoList& inFiles);

void shuffleFiles(QVector<pairFiFiL>& inFiles);
#endif // SORTFILES_H
