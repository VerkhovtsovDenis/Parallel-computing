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


unsigned simple_sum(const unsigned *V, size_t n){
    unsigned sum = 0;
    for (int i = 0; i < n; i++){
        sum += V[i];
    }
        
    return sum;
}

unsigned sum_with_omp_reduce(const unsigned *V, size_t n){
    unsigned sum = 0;

    
    #pragma omp parallel for reduction (+ :sum)

    for (int i = 0; i < n; i++){
        sum += V[i];    
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
        
        for (unsigned i = t; i < n; i += T){
            partial_sums[t] += V[i];
        }
    }

    for (unsigned i = 0; i < T; i++)
        sum += partial_sums[i];

    free(partial_sums);
    return sum;
}

#define CACHE_LINE 64

struct partial_sum_t {
    alignas(CACHE_LINE) unsigned val;
};


unsigned sum_with_aligned_omp_round_robin(const unsigned *V, size_t n){
    unsigned sum = 0;
    partial_sum_t* partial_sums;
    unsigned T;
    
    #pragma omp parallel
    {
        unsigned t = omp_get_thread_num();
        #pragma omp single
        {
            T = omp_get_num_threads();
            partial_sums = (partial_sum_t*)malloc(sizeof partial_sums[0] * T);
        }
        
        partial_sums[t].val = 0;
        
        for (unsigned i = t; i < n; i += T)
            partial_sums[t].val += V[i];
    }
    
    for (unsigned i = 0; i < T; i++)
        sum += partial_sums[i].val;
    
    free(partial_sums);
    return sum;

}

unsigned simple_sum_with_aligned(const unsigned *V, size_t n){
    unsigned sum = 0;
    for (size_t i = 0; i < n; i++)
        sum += V[i];
    return sum;
}

unsigned sum_with_omp_and_thread(const unsigned *V, size_t n){
    unsigned T;
    unsigned sum = 0;
    partial_sum_t* partial_sums;
    
    #pragma omp parallel
    {
        unsigned t = omp_get_thread_num();
        #pragma omp single
        {
            T = omp_get_num_threads();
            partial_sums = (partial_sum_t*)malloc(sizeof partial_sums[0] * T);
        }

        partial_sums[t].val = 0;
        unsigned s_t = n / T, b_t = n % T;

        if (t < b_t)
            b_t = ++s_t * t;
        else
            b_t += s_t * t;

        unsigned e_t = b_t + s_t;

        for (unsigned i = b_t; i < e_t; i++)
            partial_sums[t].val += V[i];
    }

    for (unsigned i = 0; i < T; i++)
        sum += partial_sums[i].val;
    free(partial_sums);
    return sum;
}

unsigned sum_with_cpp_methons(const unsigned *V, size_t n){
    unsigned sum = 0;
    
    unsigned T = get_num_threads(); 
    std::vector<std::thread> workers(T - 1);
    std::mutex mtx;

    auto worker_proc = [T, V, n, &sum, &mtx](unsigned t) {
        unsigned s_t = n / T, b_t = n % T;

        if (t < b_t)
            b_t = ++s_t * t;
        else
            b_t += s_t * t;

        unsigned e_t = b_t + s_t;

        unsigned my_sum = 0;

        for (unsigned i = b_t; i < e_t; i++){
            my_sum += V[i];   
        }

        std::scoped_lock l{mtx};
        sum += my_sum;
    };

    for(size_t t = 1; t < T; ++t)
        workers[t - 1] = std::thread(worker_proc, t);
    worker_proc(0);

    for (auto& worker : workers)
        worker.join();

    return sum;
}

unsigned sum_with_omp_mutex(const unsigned *V, size_t n){
    unsigned sum = 0;
    
    #pragma omp parallel
    {
        unsigned t = omp_get_thread_num();
        unsigned T = omp_get_num_threads();
        unsigned s_t = n / T, b_t = n % T;

        if (t < b_t)
            b_t = ++s_t * t;
        else
            b_t += s_t * t;

        unsigned e_t = b_t + s_t;

        unsigned my_sum = 0;

        for (unsigned i = b_t; i < e_t; i++)
            my_sum += V[i];
        
        #pragma omp critical
        {
            sum += my_sum;
        }
    }

    return sum;
}


class barrier {
    std::condition_variable cv;
    bool generation = false;
    std::mutex mtx;
    const unsigned T0;
    unsigned T;

public:
    barrier(unsigned threads) : T(threads), T0(threads) {}

    void arrive_and_wait() {
        std::unique_lock l(mtx);
        if (--T == 0) {
            T = T0;
            generation = !generation;
            cv.notify_all();
        } else {
            bool my_barrier = generation;
            cv.wait(l, [&] { return my_barrier != generation; });
        }
    }
};

unsigned sum_with_cpp_barrier(const unsigned *V, size_t n){
    
    unsigned T = get_num_threads(); // Number of threads.

    barrier sync_point(T); // Barrier for thread synchronization.
    std::vector<std::thread> workers(T - 1);

    partial_sum_t* partial_sums = (partial_sum_t*)malloc(sizeof partial_sums[0] * T);

    auto worker_proc = [&partial_sums, &sync_point, T, n, V](size_t t) {
        size_t s_t = n / T, b_t = n % T;

        if (t < b_t)
            b_t = ++s_t * t;
        else
            b_t += s_t * t;

        unsigned e_t = b_t + s_t;

        partial_sums[t].val = 0;
        for (unsigned i = b_t; i < e_t; i++)
            partial_sums[t].val += V[i];


        for (size_t step = 1, next = 2; step < T; step = next, next <<= 1)
        {
            // printf("%d: step = %d, next = %d\n", t, step, next);

            sync_point.arrive_and_wait();
            if (((t & (next - 1)) == 0 && t + step < T)){
                partial_sums[t].val += partial_sums[t + step].val;
            }
        }

    };

    for(size_t t = 1; t < T; ++t){
        workers[t - 1] = std::thread(worker_proc, t);
    }

    worker_proc(0);

    for (auto& worker : workers)
        worker.join();

    free(partial_sums);

    return partial_sums[0].val;
}


#endif
