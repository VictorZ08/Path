#ifndef CHRONOMETR_H
#define CHRONOMETR_H
#include <iostream>
#include <fstream>
#include <chrono>
#include <vector>
#include <algorithm>
#include <numeric>

using namespace std::chrono;

void writeFile(std::string &inNameFunc,
               std::string &inTime,
               double inMax,
               double inMin,
               double inAvg);

/**
    @author Victor Zaytcev
*/
class Chronometr {
public:
    //Сброс таймера
    Chronometr() : m_start(std::chrono::steady_clock::time_point::min()) { }
    //Сброс таймера
    void clear() {
        m_start = std::chrono::steady_clock::time_point::min();
    }
    //Возвращае true, если таймер работает
    bool isStarted() const {
        return (m_start.time_since_epoch() !=
        system_clock::duration(0));
    }
    //Запуск таймера
    void start() {
        m_start = std::chrono::steady_clock::now();
    }
    //Получение количества миллисекунд с момента запуска таймера
    unsigned long getMs() {
        if(isStarted()) { system_clock::duration diff;
            diff = std::chrono::steady_clock::now() - m_start;
            return ( unsigned ) (duration_cast<milliseconds>(diff).count());
        }
        return 0;
    }

private :
    std::chrono::steady_clock::time_point m_start;
};

/*********************benchmark*********************/
/**
    @brief benchmark
    benchmark для функций
    @param nameFunc название функции
    @param iterCount количество итераций
    @param func функция
    @param args аргументы функции
*/
//void benchmark(std::string nameFunc, int iterCount, Func func, Args... args )

//Для методов класса
/**
    @brief benchmark
    benchmark для методов класса
    @param obj указатель на объект класса
    @param nameFunc название метода
    @param iterCount количество итераций
    @param (Obj::*func) указатель на метод
    @param args аргументы метода
*/
template< typename Obj, typename Func, typename... Args >
void benchmark(Obj &obj,
               std::string nameFunc,
               int iterCount,
               Func (Obj::*func),
               Args... args )
{
    if( iterCount <= 0 ) {
        return;
    }
    std::vector<int> elapsedTimes;
    elapsedTimes.reserve(iterCount);
    Chronometr timer;
    for( int i = 0; i < iterCount; ++i ) {
        timer.start();
        (obj.*func)( args... );
        elapsedTimes.push_back( timer.getMs() );
        timer.clear();
    }

    double max = *std::max_element( elapsedTimes.begin(), elapsedTimes.end() ) / 1000.0;
    double min = *std::min_element( elapsedTimes.begin(), elapsedTimes.end() ) / 1000.0;
    int sum = std::accumulate( elapsedTimes.begin(), elapsedTimes.end(), 0 );
    double avg = sum / ( iterCount * 1000.0 );

    auto current_time = std::chrono::system_clock::now();
    auto current_time_t = std::chrono::system_clock::to_time_t(current_time);

    std::string strTime(30, 'q');
    std::strftime(&strTime.at(0), strTime.size(), "%d-%m-%Y %H:%M:%S", std::localtime(&current_time_t));

    auto it = std::next(strTime.begin(), strTime.find('q'));
    strTime.erase(it, strTime.end());
    strTime.shrink_to_fit();

    writeFile(nameFunc, strTime, max, min, avg);

}

/**
    @brief writeFile
    записывает измерения в файл
    @param inNameFunc название измеряемого метода
    @param inTime время в которое проводились измерения
    @param inMax максимальное значение
    @param inMin минимальное значение
    @param inAvg усреднённое значение
*/
void writeFile(std::string &inNameFunc,
               std::string &inTime,
               double inMax,
               double inMin,
               double inAvg)
{
    std::ofstream fout("C:/Users/Victor/Documents/Path/testBenchmark.txt", std::ios_base::app);
        if( fout.is_open() ) {
            std::cout << "Open" << std::endl;
        }
         else {
         std::cout << "No open" << std::endl;;
        }

        fout << "************* "<< inTime<<"**************" << std::endl;
        fout << inNameFunc << std::endl;
        fout << "Start benchmark:" << std::endl;
        fout << "    MAX: " << inMax << std::endl;
        fout << "    MIN: " << inMin << std::endl;
        fout << "    AVG: " << inAvg << std::endl;
        fout << std::endl;
        fout.close(); // закрываем файл
}
/***********************end***********************/
#endif // CHRONOMETR_H

//Для функций
/*********Example************
void test() {
    vector<int> v;
    v.reserve(2000000);
    for(int i = 0; i < 2000000; ++i) {
        v.push_back(i);
    }
}

int main()
{
    cout << "Hello World!" << endl;

    benchmark("test", 5, test);
    benchmark("test", 15, test);
    benchmark("test", 25, test);
    return 0;
}
****************************/

//Для методов
/*********Example************
//.h
class ClassTest
{
protected slots:
    //Тестируемый метод
    void test() { std::vector<int> v;
                  v.push_back(1);

private:
    //Создаём метод для тестирования
    //Удалить после измерений
    void testBenchmark();//Создаём для тестирования
}

//.cpp
void ClassTest::testBenchmark()
{
    //Если тестируем слот, тогда помещаем
    //сюда содержимое метода test();
    std::vector<int> v;
    v.push_back(1);
}

void ClassTest::test()
{
    benchmark(*this,
              "test",
              1000,
              (&ClassTest::testBenchmark));
}
******************************/
