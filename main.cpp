#include <iostream>
#include <omp.h>
#include <memory>

#include "prototype.h"



int main(int argc, char** argv){
    set_num_threads(2);

    using std::cin, std::cout, std::endl;

    auto V = std::make_unique<unsigned[]>(N);
    for(auto i = 0; i < N; ++i)
        V[i] = i;

    cout << omp_get_num_threads() << endl;
    cout << "simple_sum: " << speedtest(simple_sum, V.get(), N) << std::endl;
    cout << "sum_with_omp_reduce: " << speedtest(sum_with_omp_reduce, V.get(), N) << std::endl;
    cout << "sum_with_omp_round_robin: " << speedtest(sum_with_omp_round_robin, V.get(), N) << std::endl;

    cin.get();
    return 0;
}