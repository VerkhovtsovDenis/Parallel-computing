#ifndef prototypes
#define prototypes
#include <cstddef>
#include <thread>
#define N (1u << 27)



// Метод 1 - Простой подсчёт суммы
unsigned simple_sum(const unsigned *V, size_t n);

// Метод 2 - Подсчёт суммы через OMP с неявным распараллеливанием выполнения for
unsigned sum_with_omp_reduce(const unsigned *V, size_t n);

// Метод 3 - Подсчет суммы через OMP с явным распараллеливанием выполнения for через T частичных сумм
unsigned sum_with_omp_round_robin(const unsigned *V, size_t n);

// Метод 4 - Подсчет суммы через OMP с явным распараллеливанием выполнения for через T частичных сумм и выравниванием строки КЭШ (для исключения когерентности памяти)
unsigned sum_with_aligned_omp_round_robin(const unsigned *V, size_t n);

// Метод 5 - Тоже обычная сумма, но с выравниваем за счет size_t в for
unsigned simple_sum_with_aligned(const unsigned *V, size_t n);

// Метод 6 - Подсчет суммы через OMP (через b, e) с использованием частичных сумм
unsigned sum_with_omp_and_thread(const unsigned *V, size_t n);

// Метод 7 - Подсчет суммы через методы C++  (std::thread)  (через b, e)
unsigned sum_with_cpp_methons(const unsigned *V, size_t n);

// Метод 8 - Подсчет суммы через OMP с использованием mutex (через b, e)
unsigned sum_with_omp_mutex(const unsigned *V, size_t n);

// Метод 9 - Подсчет суммы через методы C++ с использованием барьера (Параллельная редукция)
unsigned sum_with_cpp_barrier(const unsigned *V, size_t n);

unsigned get_num_threads();
void set_num_threads(unsigned T);

unsigned speedtest(unsigned sum_funk(const unsigned *, size_t), const unsigned *V, size_t n);

#endif
