#ifndef prototypes
#define prototypes
#include <cstddef>
#include <thread>
#define N (1u << 27)


// Метод 1 - Вычсление случайных чисел простым когерентным способом
void cogerent_random(unsigned* V, size_t n, unsigned x0, unsigned xmin, unsigned xmax);

#endif
