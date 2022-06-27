#ifndef RANDOM_H
#define RANDOM_H

#include <QDateTime>

/**
    @author Victor Zaytcev
*/
class Random
{
public:
    static double randValues(const double inVal, const double inAamplituda);
    static double randOneStringAep(const double inVal);

    static QVector<QDateTime> randTimeAep(const QDateTime& inStartTime,
                                          const int inNumFilesInSet,
                                          const int inNumSets,
                                          const int inMinTimeModes,
                                          const int inMaxTimeModes,
                                          const int inTimeBetweenSets);

    static QVector<QDateTime> randTimePemi(const QDateTime& inStartTime,
                                           const quint64 inNumSets,
                                           const quint64 inTimeBetweenSets);

    static QVector<QDateTime> randTimeFixed(const QDateTime& inStartTime,
                                            const int inNumSets);

public:
    static bool checkBreak(const QDateTime& inDateTime);
};

#endif // RANDOM_H
