#ifndef RANDOMVALUES_H
#define RANDOMVALUES_H

#include <random>
#include <QDateTime>

namespace details
{
    /// True if type T is applicable by a std::uniform_int_distribution
    template<class T>
    struct is_uniform_int {
        static constexpr bool value =
                std::is_same<T,              short>::value ||
                std::is_same<T,                int>::value ||
                std::is_same<T,               long>::value ||
                std::is_same<T,          long long>::value ||
                std::is_same<T,     unsigned short>::value ||
                std::is_same<T,       unsigned int>::value ||
                std::is_same<T,      unsigned long>::value ||
                std::is_same<T, unsigned long long>::value;
    };

    /// True if type T is applicable by a std::uniform_real_distribution
    template<class T>
    struct is_uniform_real {
        static constexpr bool value =
                std::is_same<T,       float>::value ||
                std::is_same<T,      double>::value ||
                std::is_same<T, long double>::value;
    };
}

class RandomValues
{
    template <class T> using IntDist = std::uniform_int_distribution<T>;
    template <class T> using RealDist = std::uniform_real_distribution<T>;

public:
    template <class T>
    static typename std::enable_if<details::is_uniform_int<T>::value, T>::type
                                get(T from = std::numeric_limits<T>::min(),
                                T to = std::numeric_limits<T>::max())
    {
        if (from > to) std::swap(from, to);
            IntDist<T> dist{from, to};
        return dist(instance().engine());
    }

    template <class T>
    static typename std::enable_if<details::is_uniform_real<T>::value, T>::type
                                get(T from = std::numeric_limits<T>::min(),
                                T to = std::numeric_limits<T>::max())
    {
        if (from > to) std::swap(from, to);
            RealDist<T> dist{from, to};
        return dist(instance().engine());
    }

    std::mt19937& engine() { return m_mt; }

protected:
    static RandomValues& instance()
    {
        static RandomValues inst;
        return inst;
    }

private:
    std::random_device m_rd; // Устройство генерации случайных чисел
    std::mt19937 m_mt;       // Стандартный генератор случайных чисел

    RandomValues() : m_mt(m_rd()) {}
    ~RandomValues() {}
    RandomValues(const RandomValues&) = delete;
    RandomValues& operator = (const RandomValues&) = delete;
};

/*********Example************/
/*std::cout << Random::get(15, 43) << " ";*/

#endif // RANDOMVALUES_H
