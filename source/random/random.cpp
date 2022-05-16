#include "random.h"
#include "randomvalues.h"

#include "QDebug"

#define PERCENT (inTimeBetweenSets*10)/100
#define LIMIT_FOOD 1800 /*Время перекуса: 30 мин * 60 сек = 1800*/
#define ON 1
#define OFF 0

const QTime k_morningBreakBeg (11,30);
const QTime k_morningBreakEnd (12,00);
const QTime k_eveningBreakBeg (16,00);
const QTime k_eveningBreakEnd (16,30);

const quint32 k_MinTimeFile = 5;
const quint32 k_MaxTimeFile = 8;
const quint32 k_Minute = 60;

/**
    @brief Random::randValues
    генератор случайного числа с диапазоном
    @param inVal генерируемое число
    @param inAamplituda диапазон
    @return возвращает сгенерированное число
*/
double Random::randValues(const double inVal, const double inAamplituda)
{
    double val;
    return val = RandomValues::get(inVal - inAamplituda, inVal + inAamplituda);
}

/**
    @brief Random::randOneStringAep
    генератор случайного числа первой строки
    в файле АЭП
    @param inVal генерируемое число
    @return возвращает сгенерированное число
*/
double Random::randOneStringAep(const double inVal)
{
    double val;
    double percent = (inVal * 5)/100;
    val = RandomValues::get(inVal - percent, inVal + percent);
    return val - remainder(val, 0.1);
}

/**
    @brief Random::randTimeAep
    генератор случайных чисел
    @param inStartTime начальное время
    @param inNumFilesInSet количество файлов
    @param inNumSets количество комплектов
    @param inTimeBetweenSets разброс времени между комплектами
    @return возвращает контейнер сгенерированных значений
*/
QVector<QDateTime> Random::randTimeAep(const QDateTime& inStartTime,
                               const quint64 inNumFilesInSet,
                               const quint64 inNumSets,
                               const quint64 inTimeBetweenSets)
{
    quint64 folderInSet = 1;
    QVector<QDateTime> vecDateTime;
    vecDateTime.reserve((inNumFilesInSet + folderInSet) * inNumSets);

    QDateTime tmpDateTime = inStartTime;
    quint64 toSec = 0;
    bool fOneComlect = false;
    vecDateTime.push_back(inStartTime);

    for(quint64 i = 0; i < inNumSets; ++i) {
        if(fOneComlect == true) {
            toSec = RandomValues::get(inTimeBetweenSets - PERCENT, inTimeBetweenSets + PERCENT)*k_Minute;
            tmpDateTime = tmpDateTime.addSecs(toSec);
            while (Random::checkBreak(tmpDateTime)) {
                toSec = RandomValues::get(inTimeBetweenSets - PERCENT, inTimeBetweenSets + PERCENT)*k_Minute;
                tmpDateTime = tmpDateTime.addSecs(toSec);
            }
            vecDateTime.push_back(tmpDateTime);
        }
        fOneComlect = true;

        for(quint64 i = 0; i < inNumFilesInSet; ++i) {
           toSec = RandomValues::get(k_MinTimeFile, k_MaxTimeFile)*k_Minute;
           tmpDateTime = tmpDateTime.addSecs(toSec);
           while (Random::checkBreak(tmpDateTime)) {
               toSec = RandomValues::get(k_MinTimeFile, k_MaxTimeFile)*k_Minute;
               tmpDateTime = tmpDateTime.addSecs(toSec);
           }
           vecDateTime.push_back(tmpDateTime);
        }
    }
return vecDateTime;
}

/**
    @brief Random::checkBreak
    проверяет сгенерированное время на время отдых
    @param inDateTime начальное время
    @return возвращает true в случае совпадения
*/
bool Random::checkBreak(QDateTime& inDateTime)
{
    if((inDateTime.time() > k_morningBreakBeg && inDateTime.time() < k_morningBreakEnd) ||
       (inDateTime.time() > k_eveningBreakBeg && inDateTime.time() < k_eveningBreakEnd)) {
        return true;
    }
    return false;
}

/**
    @brief Random::randTimePemi
    генератор случайных чисел
    @param inStartTime начальное время
    @param inNumSets количество комплектов
    @param inTimeBetweenSets разброс времени между комплектами
    @return возвращает контейнер сгенерированных значений
*/
QVector<QDateTime> Random::randTimePemi(const QDateTime& inStartTime,
                                        const quint64 inNumSets,
                                        const quint64 inTimeBetweenSets)
{
    QVector<QDateTime> vecDateTime;
    vecDateTime.reserve(inNumSets);
    QDateTime tmpDateTime = inStartTime;

    for(quint64 i = 0; i < inNumSets; ++i) {
        quint64 toSec = RandomValues::get(inTimeBetweenSets - PERCENT,
                                          inTimeBetweenSets + PERCENT)*k_Minute;
        tmpDateTime = tmpDateTime.addSecs(toSec);
        vecDateTime.push_back(tmpDateTime);
        if(vecDateTime.at(i).time() > k_morningBreakBeg && vecDateTime.at(i).time() < k_morningBreakEnd)
            Random::insertTime(vecDateTime, i);
        else if(vecDateTime.at(i).time() > k_eveningBreakBeg && vecDateTime.at(i).time() < k_eveningBreakEnd)
            Random::insertTime(vecDateTime, i);

        qDebug()<<vecDateTime.at(i);
    }
return vecDateTime;
}

/**
    @brief Random::insertTime
    вставляем время в контейнер
    @param inDateTime контейнер времени
    @param inCount
*/
void Random::insertTime(QVector<QDateTime>& inDateTime,
                        const quint64 inCount)
{
    QDateTime dt = inDateTime.at(inCount).addSecs(LIMIT_FOOD);
    inDateTime.pop_back();
    inDateTime.push_back(dt);
}
