#ifndef prototypes_cpp
#define prototypes_cpp

#include <cstddef>
#include <iostream>
#include <omp.h>
#include <type_traits>
#include <vector>
#include <condition_variable>
#include <mutex> 
#include "num_threads.h"


#if defined (__GNUC__)&&__GNUC__<14
   #define hardware_destructive_interference_size 64
#else
   #include <thread>
   using std::hardware_destructive_interference_size;
#endif



template <class T = unsigned>
class affine_monoid {
    T m_a = 1, m_b = 0;

public:
    affine_monoid() = default;
    affine_monoid(T a, T b) : m_a(a), m_b(b) {}
    affine_monoid& operator*=(const affine_monoid& r) {
        m_b += m_a * r.m_b;
        m_a *= r.m_a;
        return *this;
    }
    T operator()(T x) const {
        return m_a * x + m_b;
    }
};

template <class T = unsigned>
class multoplicative_monoid {
    T x = T{1};

public:
    multoplicative_monoid() = default;
    explicit multoplicative_monoid(T val) : x(val) {}
    multoplicative_monoid& operator*=(multoplicative_monoid right) {
        x *= right.x; // Операция умножения
        return *this;
    }
    explicit operator T() const {
        return x;
    }
};

template <class T, std::unsigned_integral U>
T pow(T x, U n) {
    auto r = T();

    while (n > 0) {
        if (n & 1 != 0)
            r *= x;
        x *= x;
        n >>= 1;
    }

    return r;
}

unsigned r(unsigned xi, unsigned x_min, unsigned x_delta){
    if (xi < x_delta)
        return x_min + xi;
    else
        return x_min + (xi % x_delta);
}

void cogerent_random(unsigned* V, size_t n, unsigned x0, unsigned xmin, unsigned xmax) {
    const unsigned A = 0x8088405;
    const unsigned B = 1;
    const unsigned C = 1 << 31; // 2^32

    if (xmin >= xmax || n == 0 || C == 0)
        return;
        
        
    unsigned T = get_num_threads(); // Число потоков.
    
    std::vector<std::thread> workers(T-1);

    unsigned x_delta = xmax - xmin;
    
    auto process = [&V, x0, A, B, xmin, x_delta, n, T](unsigned t){
        unsigned s_t = n / T;
        unsigned b_t = n % T;
        unsigned e_t = 0;
        
        if (t < b_t)
            b_t = ++s_t * t;
        else
            b_t += s_t * t;
        
        e_t = b_t + s_t;

        unsigned xi = x0;
        affine_monoid<unsigned> m(A, B);
        auto my_m = pow(m, b_t);
        
        for (auto i = b_t; i < e_t; i++){
            xi = my_m(x0);   // Генерация нового числа
            V[i] = r(xi, xmin, x_delta);               // Запись результата
            my_m *= m;
        }

    };
    
    for (size_t t = 1; t < T; ++t) {
        workers[t - 1] = std::thread(process, t);
    }
    process(0);

    for (auto& worker : workers) {
        worker.join();
    }
    
    return;
}


#endif
