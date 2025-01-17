#include <iostream>
#include <omp.h>
#include <memory>

#include "prototype.h"



int main(int argc, char** argv){
    set_num_threads(4);

    using std::cin, std::cout, std::endl;
    
    cout << "N = " << N << endl;

    auto V = std::make_unique<unsigned[]>(N);
    for(auto i = 0; i < N; ++i)
        V[i] = i;

    cout << "simple_sum: " << speedtest(simple_sum, V.get(), N) << std::endl;
    cout << "sum_with_omp_reduce: " << speedtest(sum_with_omp_reduce, V.get(), N) << std::endl;
    cout << "sum_with_omp_round_robin: " << speedtest(sum_with_omp_round_robin, V.get(), N) << std::endl;
    cout << "sum_with_aligned_omp_round_robin: " << speedtest(sum_with_aligned_omp_round_robin, V.get(), N) << std::endl;
    cout << "simple_sum_with_aligned: " << speedtest(simple_sum_with_aligned, V.get(), N) << std::endl;
    cout << "sum_with_omp_and_thread: " << speedtest(sum_with_omp_and_thread, V.get(), N) << std::endl;
    cout << "sum_with_cpp_methons: " << speedtest(sum_with_cpp_methons, V.get(), N) << std::endl;
    cout << "sum_with_omp_mutex: " << speedtest(sum_with_omp_mutex, V.get(), N) << std::endl;
    cout << "sum_with_cpp_barrier: " << speedtest(sum_with_cpp_barrier, V.get(), N) << std::endl;

    return 0;
}