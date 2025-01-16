#ifndef prototypes_cpp
#define prototypes_cpp

#include <cstddef>
#include <iostream>
#include <omp.h>
#include <type_traits>

#if defined (__GNUC__)&&__GNUC__<14
   #define hardware_destructive_interference_size 64
#else
   #include <thread>
   using std::hardware_destructive_interference_size;
#endif


// #ifndef __cplusplus
//     typedef unsigned (*sum_ptr) (const unsigned* v, size_t n);
// #else 
//     template <class F> //include type_traits
//     concept sum_callable = std::is_invocable_r<unsigned, F, const unsigned*, size_t>;
// #endif

static unsigned g_num_threads = std::thread::hardware_concurrency();

#define CACHE_LINE 64
struct partial_sum_t {
    alignas(CACHE_LINE) unsigned val;
};

void set_num_threads(unsigned T) {
    g_num_threads = T;
    omp_set_num_threads(T);
};

unsigned get_num_threads() {
    return g_num_threads;
};

unsigned speedtest(unsigned (*sum_funk)(const unsigned *, size_t ), const unsigned *V, size_t n){
    auto t0 = omp_get_wtime();
    sum_funk(V, n);
    auto t1 = omp_get_wtime();
    return (t1 - t0) * 1E+3;
}


unsigned simple_sum(const unsigned *V, size_t n){
    unsigned sum = 0;
    for (int i = 0; i < n; i++){
        sum += V[i];
    }
        
    return sum;
}

unsigned sum_with_omp_reduce(const unsigned *V, size_t n){
    unsigned sum = 0;

    using std::cout, std::endl;
    // cout << omp_get_num_threads() << endl;
    
    #pragma omp parallel for reduction (+ :sum)
    for (int i = 0; i < n; i++){
        sum += V[i];    
        // cout << omp_get_thread_num() << endl;
    }
    return sum;
}

unsigned sum_with_omp_round_robin(const unsigned *V, size_t n){
    
    unsigned sum = 0;
    unsigned* partial_sums;
    unsigned T;

    #pragma omp parallel
    {
        T = omp_get_num_threads();
        unsigned t = omp_get_thread_num();
        #pragma omp single
        {
            partial_sums = (unsigned*) calloc(sizeof V[0], T);
        }

        for (unsigned i = t; i < n; i += T)
            partial_sums[t] += V[i];
    }

    for (unsigned i = 0; i < T; i++)
        sum += partial_sums[i];

    free(partial_sums);
    return sum;
}

unsigned sum_with_aligned_omp_round_robin(const unsigned *V, size_t n);

unsigned simple_sum_with_aligned(const unsigned *V, size_t n);

unsigned sum_with_omp_and_thread(const unsigned *V, size_t n);

unsigned sum_with_cpp_methons(const unsigned *V, size_t n);

unsigned sum_with_omp_mutex(const unsigned *V, size_t n);

unsigned sum_with_cpp_barrier(const unsigned *V, size_t n);


#endif
